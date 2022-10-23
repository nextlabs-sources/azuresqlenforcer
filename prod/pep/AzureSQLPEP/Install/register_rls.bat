@echo off
setlocal enabledelayedexpansion
set EXE="%~dp0\NXL2RLSSrv.exe"
set SERVICE=NXL2RLSSrv
if "%1"=="unreg" (
echo unregister service %SERVICE%
::rem first stop the service
set STOPSRV_CMD=sc stop %SERVICE%
echo !STOPSRV_CMD!
!STOPSRV_CMD!

:rem second delete service
set UNREG_CMD=sc delete %SERVICE%
echo !UNREG_CMD!
!UNREG_CMD!

) else (
:rem crate service
set CREATE=sc create %SERVICE% binPath=%EXE% start=auto
echo !CREATE!
!CREATE!

:rem set the description of the service
set DESC=sc description %SERVICE%  "Nextlabs Entitlement Management for Azure SQL--Filter/Block module"
echo !DESC!
!DESC!

:rem set recovery
set RECOVER=sc failure %SERVICE% reset=86400 actions=restart/1000/restart/1000/restart/1000
echo !RECOVER!
!RECOVER!

:rem start service
set START=sc start %SERVICE%
echo !START!
!START!
)