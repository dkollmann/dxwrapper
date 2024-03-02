#pragma once

#ifdef DeviceCapabilities
#undef DeviceCapabilities
#endif

#define VISIT_PROCS_WINSPOOL(visit) \
	visit(ADVANCEDSETUPDIALOG, jmpaddr) \
	visit(AdvancedSetupDialog, jmpaddr) \
	visit(ConvertAnsiDevModeToUnicodeDevmode, jmpaddr) \
	visit(ConvertUnicodeDevModeToAnsiDevmode, jmpaddr) \
	visit(DEVICEMODE, jmpaddr) \
	visit(DeviceMode, jmpaddr) \
	visit(DocumentEvent, jmpaddr) \
	visit(PerfClose, jmpaddr) \
	visit(PerfCollect, jmpaddr) \
	visit(PerfOpen, jmpaddr) \
	visit(QueryColorProfile, jmpaddr) \
	visit(QueryRemoteFonts, jmpaddr) \
	visit(QuerySpoolMode, jmpaddr) \
	visit(SpoolerDevQueryPrintW, jmpaddr) \
	visit(StartDocDlgW, jmpaddr) \
	visit(AbortPrinter, jmpaddr) \
	visit(AddFormA, jmpaddr) \
	visit(AddFormW, jmpaddr) \
	visit(AddJobA, jmpaddr) \
	visit(AddJobW, jmpaddr) \
	visit(AddMonitorA, jmpaddr) \
	visit(AddMonitorW, jmpaddr) \
	visit(AddPortA, jmpaddr) \
	visit(AddPortExA, jmpaddr) \
	visit(AddPortExW, jmpaddr) \
	visit(AddPortW, jmpaddr) \
	visit(AddPrintProcessorA, jmpaddr) \
	visit(AddPrintProcessorW, jmpaddr) \
	visit(AddPrintProvidorA, jmpaddr) \
	visit(AddPrintProvidorW, jmpaddr) \
	visit(AddPrinterA, jmpaddr) \
	visit(AddPrinterConnection2A, jmpaddr) \
	visit(AddPrinterConnection2W, jmpaddr) \
	visit(AddPrinterConnectionA, jmpaddr) \
	visit(AddPrinterConnectionW, jmpaddr) \
	visit(AddPrinterDriverA, jmpaddr) \
	visit(AddPrinterDriverExA, jmpaddr) \
	visit(AddPrinterDriverExW, jmpaddr) \
	visit(AddPrinterDriverW, jmpaddr) \
	visit(AddPrinterW, jmpaddr) \
	visit(AdvancedDocumentPropertiesA, jmpaddr) \
	visit(AdvancedDocumentPropertiesW, jmpaddr) \
	visit(ClosePrinter, jmpaddr) \
	visit(CloseSpoolFileHandle, jmpaddr) \
	visit(CommitSpoolData, jmpaddr) \
	visit(ConfigurePortA, jmpaddr) \
	visit(ConfigurePortW, jmpaddr) \
	visit(ConnectToPrinterDlg, jmpaddr) \
	visit(CorePrinterDriverInstalledA, jmpaddr) \
	visit(CorePrinterDriverInstalledW, jmpaddr) \
	visit(CreatePrintAsyncNotifyChannel, jmpaddr) \
	visit(CreatePrinterIC, jmpaddr) \
	visit(DeleteFormA, jmpaddr) \
	visit(DeleteFormW, jmpaddr) \
	visit(DeleteJobNamedProperty, jmpaddr) \
	visit(DeleteMonitorA, jmpaddr) \
	visit(DeleteMonitorW, jmpaddr) \
	visit(DeletePortA, jmpaddr) \
	visit(DeletePortW, jmpaddr) \
	visit(DeletePrintProcessorA, jmpaddr) \
	visit(DeletePrintProcessorW, jmpaddr) \
	visit(DeletePrintProvidorA, jmpaddr) \
	visit(DeletePrintProvidorW, jmpaddr) \
	visit(DeletePrinter, jmpaddr) \
	visit(DeletePrinterConnectionA, jmpaddr) \
	visit(DeletePrinterConnectionW, jmpaddr) \
	visit(DeletePrinterDataA, jmpaddr) \
	visit(DeletePrinterDataExA, jmpaddr) \
	visit(DeletePrinterDataExW, jmpaddr) \
	visit(DeletePrinterDataW, jmpaddr) \
	visit(DeletePrinterDriverA, jmpaddr) \
	visit(DeletePrinterDriverExA, jmpaddr) \
	visit(DeletePrinterDriverExW, jmpaddr) \
	visit(DeletePrinterDriverPackageA, jmpaddr) \
	visit(DeletePrinterDriverPackageW, jmpaddr) \
	visit(DeletePrinterDriverW, jmpaddr) \
	visit(DeletePrinterIC, jmpaddr) \
	visit(DeletePrinterKeyA, jmpaddr) \
	visit(DeletePrinterKeyW, jmpaddr) \
	visit(DevQueryPrint, jmpaddr) \
	visit(DevQueryPrintEx, jmpaddr) \
	visit(DeviceCapabilities, jmpaddr) \
	visit(DeviceCapabilitiesA, jmpaddr) \
	visit(DeviceCapabilitiesW, jmpaddr) \
	visit(DevicePropertySheets, jmpaddr) \
	visit(DocumentPropertiesA, jmpaddr) \
	visit(DocumentPropertiesW, jmpaddr) \
	visit(DocumentPropertySheets, jmpaddr) \
	visit(EXTDEVICEMODE, jmpaddr) \
	visit(EndDocPrinter, jmpaddr) \
	visit(EndPagePrinter, jmpaddr) \
	visit(EnumFormsA, jmpaddr) \
	visit(EnumFormsW, jmpaddr) \
	visit(EnumJobNamedProperties, jmpaddr) \
	visit(EnumJobsA, jmpaddr) \
	visit(EnumJobsW, jmpaddr) \
	visit(EnumMonitorsA, jmpaddr) \
	visit(EnumMonitorsW, jmpaddr) \
	visit(EnumPortsA, jmpaddr) \
	visit(GetDefaultPrinterA, jmpaddr) \
	visit(SetDefaultPrinterA, jmpaddr) \
	visit(GetDefaultPrinterW, jmpaddr) \
	visit(SetDefaultPrinterW, jmpaddr) \
	visit(EnumPortsW, jmpaddr) \
	visit(EnumPrintProcessorDatatypesA, jmpaddr) \
	visit(EnumPrintProcessorDatatypesW, jmpaddr) \
	visit(EnumPrintProcessorsA, jmpaddr) \
	visit(EnumPrintProcessorsW, jmpaddr) \
	visit(EnumPrinterDataA, jmpaddr) \
	visit(EnumPrinterDataExA, jmpaddr) \
	visit(EnumPrinterDataExW, jmpaddr) \
	visit(EnumPrinterDataW, jmpaddr) \
	visit(EnumPrinterDriversA, jmpaddr) \
	visit(EnumPrinterDriversW, jmpaddr) \
	visit(EnumPrinterKeyA, jmpaddr) \
	visit(EnumPrinterKeyW, jmpaddr) \
	visit(EnumPrintersA, jmpaddr) \
	visit(EnumPrintersW, jmpaddr) \
	visit(ExtDeviceMode, jmpaddr) \
	visit(FindClosePrinterChangeNotification, jmpaddr) \
	visit(FindFirstPrinterChangeNotification, jmpaddr) \
	visit(FindNextPrinterChangeNotification, jmpaddr) \
	visit(FlushPrinter, jmpaddr) \
	visit(FreePrintNamedPropertyArray, jmpaddr) \
	visit(FreePrintPropertyValue, jmpaddr) \
	visit(FreePrinterNotifyInfo, jmpaddr) \
	visit(GetCorePrinterDriversA, jmpaddr) \
	visit(GetCorePrinterDriversW, jmpaddr) \
	visit(GetFormA, jmpaddr) \
	visit(GetFormW, jmpaddr) \
	visit(GetJobA, jmpaddr) \
	visit(GetJobNamedPropertyValue, jmpaddr) \
	visit(GetJobW, jmpaddr) \
	visit(GetPrintExecutionData, jmpaddr) \
	visit(GetPrintOutputInfo, jmpaddr) \
	visit(GetPrintProcessorDirectoryA, jmpaddr) \
	visit(GetPrintProcessorDirectoryW, jmpaddr) \
	visit(GetPrinterA, jmpaddr) \
	visit(GetPrinterDataA, jmpaddr) \
	visit(GetPrinterDataExA, jmpaddr) \
	visit(GetPrinterDataExW, jmpaddr) \
	visit(GetPrinterDataW, jmpaddr) \
	visit(GetPrinterDriver2A, jmpaddr) \
	visit(GetPrinterDriver2W, jmpaddr) \
	visit(GetPrinterDriverA, jmpaddr) \
	visit(GetPrinterDriverDirectoryA, jmpaddr) \
	visit(GetPrinterDriverDirectoryW, jmpaddr) \
	visit(GetPrinterDriverPackagePathA, jmpaddr) \
	visit(GetPrinterDriverPackagePathW, jmpaddr) \
	visit(GetPrinterDriverW, jmpaddr) \
	visit(GetPrinterW, jmpaddr) \
	visit(GetSpoolFileHandle, jmpaddr) \
	visit(InstallPrinterDriverFromPackageA, jmpaddr) \
	visit(InstallPrinterDriverFromPackageW, jmpaddr) \
	visit(IsValidDevmodeA, jmpaddr) \
	visit(IsValidDevmodeW, jmpaddr) \
	visit(OpenPrinter2A, jmpaddr) \
	visit(OpenPrinter2W, jmpaddr) \
	visit(OpenPrinterA, jmpaddr) \
	visit(OpenPrinterW, jmpaddr) \
	visit(PlayGdiScriptOnPrinterIC, jmpaddr) \
	visit(PrinterMessageBoxA, jmpaddr) \
	visit(PrinterMessageBoxW, jmpaddr) \
	visit(PrinterProperties, jmpaddr) \
	visit(ReadPrinter, jmpaddr) \
	visit(RegisterForPrintAsyncNotifications, jmpaddr) \
	visit(ReportJobProcessingProgress, jmpaddr) \
	visit(ResetPrinterA, jmpaddr) \
	visit(ResetPrinterW, jmpaddr) \
	visit(ScheduleJob, jmpaddr) \
	visit(SeekPrinter, jmpaddr) \
	visit(SetFormA, jmpaddr) \
	visit(SetFormW, jmpaddr) \
	visit(SetJobA, jmpaddr) \
	visit(SetJobNamedProperty, jmpaddr) \
	visit(SetJobW, jmpaddr) \
	visit(SetPortA, jmpaddr) \
	visit(SetPortW, jmpaddr) \
	visit(SetPrinterA, jmpaddr) \
	visit(SetPrinterDataA, jmpaddr) \
	visit(SetPrinterDataExA, jmpaddr) \
	visit(SetPrinterDataExW, jmpaddr) \
	visit(SetPrinterDataW, jmpaddr) \
	visit(SetPrinterW, jmpaddr) \
	visit(SplDriverUnloadComplete, jmpaddr) \
	visit(SpoolerPrinterEvent, jmpaddr) \
	visit(StartDocDlgA, jmpaddr) \
	visit(StartDocPrinterA, jmpaddr) \
	visit(StartDocPrinterW, jmpaddr) \
	visit(StartPagePrinter, jmpaddr) \
	visit(UnRegisterForPrintAsyncNotifications, jmpaddr) \
	visit(UploadPrinterDriverPackageA, jmpaddr) \
	visit(UploadPrinterDriverPackageW, jmpaddr) \
	visit(WaitForPrinterChange, jmpaddr) \
	visit(WritePrinter, jmpaddr) \
	visit(XcvDataW, jmpaddr)

#ifdef PROC_CLASS
PROC_CLASS(winspool, dll, VISIT_PROCS_WINSPOOL, VISIT_PROCS_BLANK, VISIT_PROCS_BLANK)
#endif
