' VBScript 예제: C 프로그램 실행 및 결과 처리
' VBScript to execute C program with enhanced functionality

' Variables
Set objShell = CreateObject("WScript.Shell")
Set objFSO = CreateObject("Scripting.FileSystemObject")
logFilePath = "C:\Users\gwangho\source\repos\BondStockSys\x64\Release\log.txt" 
programName = "C:\Users\gwangho\source\repos\BondStockSys\x64\Release\BondStockSys.exe"
' Function to log messages
Sub LogMessage(message)
    Set logFile = objFSO.OpenTextFile(logFilePath, 8, True)
    logFile.WriteLine Now & " - " & message
    logFile.Close
End Sub

' Function to run the C program
Function RunProgram(programName)
    On Error Resume Next
    startTime = Timer
    result = objShell.Run(programName, 1, True)
    runTime = Timer - startTime
    
    If result = 0 Then
        LogMessage programName & " executed successfully in " & runTime & " seconds."
        WScript.Echo programName & " executed successfully."
    Else
        LogMessage "Error occurred while executing " & programName
        WScript.Echo "Error occurred while executing " & programName
    End If
    
    On Error GoTo 0
End Function

' Function to ask user input
Function GetUserChoice()
    WScript.Echo "Select an option:"
    WScript.Echo "1. Execute BondStockSys.exe"
    WScript.Echo "2. Exit"
    choice = InputBox("Enter your choice (1 or 2):", "Program Execution")
    GetUserChoice = choice
End Function

' Main script execution
userChoice = GetUserChoice()

If userChoice = "1" Then
    RunProgram programName
Else
    LogMessage "User chose to exit the script."
    WScript.Echo "Exiting the script."
End If
