# Basis ist das offizielle Phasar-Image
FROM pdschbrt/phasar:latest

# GDB und GDBServer installieren
# Versuche die Installation direkt. Wenn das Basisimage als root läuft,
# oder der Standarduser sudo-Rechte ohne Passwort hat, könnte das klappen.
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    gdb \
    gdbserver && \
    rm -rf /var/lib/apt/lists/*

# Den letzten USER-Befehl erstmal weglassen, bis wir wissen, welcher User im Basisimage aktiv ist.
# Du kannst später mit 'docker run --rm -it phasar-gdb-env whoami' prüfen, welcher User aktiv ist.