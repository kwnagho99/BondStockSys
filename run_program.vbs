' VBScript 예제: C 프로그램 실행 및 결과 처리

' C 프로그램 실행
Set objShell = CreateObject("WScript.Shell")
result = objShell.Run("BondStockSys.exe", 1, True)

' 실행 결과 확인
If result = 0 Then
    WScript.Echo "C 프로그램이 성공적으로 실행되었습니다."
Else
    WScript.Echo "C 프로그램 실행 중 오류가 발생했습니다."
End If

' 추가 작업 (예: 로그 파일 읽기)
Set objFSO = CreateObject("Scripting.FileSystemObject")
Set logFile = objFSO.OpenTextFile("log.txt", 1)

' 로그 파일 내용 출력
Do Until logFile.AtEndOfStream
    WScript.Echo logFile.ReadLine
Loop

logFile.Close
