#!/usr/bin/env bash
set -euo pipefail

if [ -z "$1" ]; then
  echo "Fehler: Es wurde kein Programm zur Messung angegeben."
  echo "Aufruf: sudo $0 <pfad-zum-programm> [ANZAHL_LAUEFE]"
  exit 1
fi

BIN="$1"
RUNS="${2:-100}"
RAPL="/sys/class/powercap/intel-rapl/intel-rapl:0/energy_uj"

if [ ! -x "$BIN" ]; then
  echo "Programm nicht gefunden oder nicht ausführbar: $BIN"
  exit 1
fi

if [ ! -r "$RAPL" ]; then
  echo "RAPL-Pfad nicht lesbar: $RAPL"
  exit 1
fi

echo "Messung für: $BIN"
echo "Anzahl der Läufe: $RUNS"
echo "Verwendeter RAPL-Knoten: $RAPL"
echo

total=0
total_time_ms=0

for i in $(seq 1 "$RUNS"); do
  printf "Durchlauf %d/%d …\n" "$i" "$RUNS"
  before=$(cat "$RAPL")

  start_ns=$(date +%s%N)
  start_ms=$(date +%s%3N)

  "$BIN" >/dev/null

  end_ns=$(date +%s%N)
  end_ms=$(date +%s%3N)
  after=$(cat "$RAPL")

  delta=$(awk -v a="$after" -v b="$before" 'BEGIN{
    max = 18446744073709551616;
    if (a >= b) print a - b;
    else print a + max - b;
  }')
  
  delta_ns=$((end_ns - start_ns))
  if [ "$delta_ns" -ge 0 ]; then
    delta_ms=$((delta_ns / 1000000))
  else
    delta_ms=$((end_ms - start_ms))
  fi

  echo "  Energie Durchlauf $i: ${delta} µJ, Laufzeit: ${delta_ms} ms"

  total=$(awk -v t="$total" -v d="$delta" 'BEGIN{printf "%.0f", t + d}')
  total_time_ms=$((total_time_ms + delta_ms))
done

avg_uj=$(awk -v total="$total" -v runs="$RUNS" 'BEGIN{printf "%.2f", total / runs}')
avg_j=$(awk -v total="$total" -v runs="$RUNS" 'BEGIN{printf "%.6f", total / runs / 1e6}')
avg_time_ms=$(awk -v t="$total_time_ms" -v runs="$RUNS" 'BEGIN{printf "%.2f", t / runs}')
total_time_s=$(awk -v t="$total_time_ms" 'BEGIN{printf "%.6f", t / 1000}')

echo
echo "===== Zusammenfassung ====="
echo "Programm: $BIN"
echo "Anzahl Durchläufe: $RUNS"
echo "Gesamtenergie: $total µJ"
echo "Durchschnitt pro Durchlauf: $avg_uj µJ  (= $avg_j J)"
echo "Gesamtzeit: ${total_time_ms} ms (= ${total_time_s} s)"
echo "Durchschnittliche Laufzeit pro Durchlauf: ${avg_time_ms} ms"
