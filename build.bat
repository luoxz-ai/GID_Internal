set UE4=C:\Program Files\Epic Games\UE_4.14
"%UE4%/Engine/Build/BatchFiles/RunUAT.bat" BuildPlugin -plugin=%CD%/UnrealCV.uplugin -package=%CD%/built -rocket -targetplatforms=Win64
