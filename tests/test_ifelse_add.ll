; ModuleID = 'tests/test_ifelse_add.cpp'
source_filename = "tests/test_ifelse_add.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #0 !dbg !10 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  call void @llvm.dbg.declare(metadata ptr %2, metadata !15, metadata !DIExpression()), !dbg !16
  store i32 5, ptr %2, align 4, !dbg !16
  call void @llvm.dbg.declare(metadata ptr %3, metadata !17, metadata !DIExpression()), !dbg !18
  store i32 10, ptr %3, align 4, !dbg !18
  call void @llvm.dbg.declare(metadata ptr %4, metadata !19, metadata !DIExpression()), !dbg !20
  store i32 0, ptr %4, align 4, !dbg !20
  %5 = load i32, ptr %2, align 4, !dbg !21
  %6 = load i32, ptr %3, align 4, !dbg !23
  %7 = icmp slt i32 %5, %6, !dbg !24
  br i1 %7, label %8, label %12, !dbg !25

8:                                                ; preds = %0
  %9 = load i32, ptr %2, align 4, !dbg !26
  %10 = load i32, ptr %3, align 4, !dbg !28
  %11 = add nsw i32 %9, %10, !dbg !29
  store i32 %11, ptr %4, align 4, !dbg !30
  br label %18, !dbg !31

12:                                               ; preds = %0
  %13 = load i32, ptr %2, align 4, !dbg !32
  %14 = load i32, ptr %3, align 4, !dbg !34
  %15 = add nsw i32 %13, %14, !dbg !35
  store i32 %15, ptr %4, align 4, !dbg !36
  %16 = load i32, ptr %4, align 4, !dbg !37
  %17 = add nsw i32 %16, 1, !dbg !38
  store i32 %17, ptr %4, align 4, !dbg !39
  br label %18

18:                                               ; preds = %12, %8
  %19 = load i32, ptr %4, align 4, !dbg !40
  %20 = add nsw i32 %19, 1, !dbg !41
  store i32 %20, ptr %4, align 4, !dbg !42
  %21 = load i32, ptr %4, align 4, !dbg !43
  ret i32 %21, !dbg !44
}

; Function Attrs: nocallback nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind readnone speculatable willreturn }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!2, !3, !4, !5, !6, !7, !8}
!llvm.ident = !{!9}

!0 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !1, producer: "Ubuntu clang version 15.0.7", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "tests/test_ifelse_add.cpp", directory: "/workspaces/DataflowEnergyPredictor", checksumkind: CSK_MD5, checksum: "8d9a336f7b63bfb2dad674720e695391")
!2 = !{i32 7, !"Dwarf Version", i32 5}
!3 = !{i32 2, !"Debug Info Version", i32 3}
!4 = !{i32 1, !"wchar_size", i32 4}
!5 = !{i32 7, !"PIC Level", i32 2}
!6 = !{i32 7, !"PIE Level", i32 2}
!7 = !{i32 7, !"uwtable", i32 2}
!8 = !{i32 7, !"frame-pointer", i32 2}
!9 = !{!"Ubuntu clang version 15.0.7"}
!10 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 1, type: !11, scopeLine: 2, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !14)
!11 = !DISubroutineType(types: !12)
!12 = !{!13}
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!14 = !{}
!15 = !DILocalVariable(name: "a", scope: !10, file: !1, line: 3, type: !13)
!16 = !DILocation(line: 3, column: 9, scope: !10)
!17 = !DILocalVariable(name: "b", scope: !10, file: !1, line: 4, type: !13)
!18 = !DILocation(line: 4, column: 9, scope: !10)
!19 = !DILocalVariable(name: "c", scope: !10, file: !1, line: 5, type: !13)
!20 = !DILocation(line: 5, column: 9, scope: !10)
!21 = !DILocation(line: 7, column: 9, scope: !22)
!22 = distinct !DILexicalBlock(scope: !10, file: !1, line: 7, column: 9)
!23 = !DILocation(line: 7, column: 13, scope: !22)
!24 = !DILocation(line: 7, column: 11, scope: !22)
!25 = !DILocation(line: 7, column: 9, scope: !10)
!26 = !DILocation(line: 9, column: 13, scope: !27)
!27 = distinct !DILexicalBlock(scope: !22, file: !1, line: 8, column: 5)
!28 = !DILocation(line: 9, column: 17, scope: !27)
!29 = !DILocation(line: 9, column: 15, scope: !27)
!30 = !DILocation(line: 9, column: 11, scope: !27)
!31 = !DILocation(line: 10, column: 5, scope: !27)
!32 = !DILocation(line: 13, column: 13, scope: !33)
!33 = distinct !DILexicalBlock(scope: !22, file: !1, line: 12, column: 5)
!34 = !DILocation(line: 13, column: 17, scope: !33)
!35 = !DILocation(line: 13, column: 15, scope: !33)
!36 = !DILocation(line: 13, column: 11, scope: !33)
!37 = !DILocation(line: 14, column: 13, scope: !33)
!38 = !DILocation(line: 14, column: 15, scope: !33)
!39 = !DILocation(line: 14, column: 11, scope: !33)
!40 = !DILocation(line: 16, column: 9, scope: !10)
!41 = !DILocation(line: 16, column: 11, scope: !10)
!42 = !DILocation(line: 16, column: 7, scope: !10)
!43 = !DILocation(line: 17, column: 12, scope: !10)
!44 = !DILocation(line: 17, column: 5, scope: !10)
