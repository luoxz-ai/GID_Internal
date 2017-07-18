#include "UnrealCVPrivate.h"
#include "ObjectHandler.h"
#include "ObjectPainter.h"
#include "Version.h"
#include "StaticMeshResources.h"
#include <string>
void FObjectCommandHandler::RegisterCommands()
{
	FDispatcherDelegate Cmd;
	FString Help;

	Cmd = FDispatcherDelegate::CreateRaw(this, &FObjectCommandHandler::HelloWorld);
	Help = "Hello World";
	CommandDispatcher->BindCommand(TEXT("vget /object/helloworld"), Cmd, Help);

	Cmd = FDispatcherDelegate::CreateRaw(this, &FObjectCommandHandler::GetObjects);
	Help = "Get the name of all objects";
	CommandDispatcher->BindCommand(TEXT("vget /objects"), Cmd, Help);

	// The order matters
	// Cmd = FDispatcherDelegate::CreateRaw(this, &FObjectCommandHandler::CurrentObjectHandler); // Redirect to current
	// CommandDispatcher->BindCommand(TEXT("[str] /object/_/[str]"), Cmd, "Get current object");

	Cmd = FDispatcherDelegate::CreateRaw(this, &FObjectCommandHandler::GetObjectColor);
	Help = "Get the labeling color of an object (used in object instance mask)";
	CommandDispatcher->BindCommand(TEXT("vget /object/[str]/color"), Cmd, Help);

	Cmd = FDispatcherDelegate::CreateRaw(this, &FObjectCommandHandler::SetObjectColor);
	Help = "Set the labeling color of an object [r, g, b]";
	CommandDispatcher->BindCommand(TEXT("vset /object/[str]/color [uint] [uint] [uint]"), Cmd, Help);

	Cmd = FDispatcherDelegate::CreateRaw(this, &FObjectCommandHandler::SetObjectMoBility);
	Help = "Set the labeling color of an object [r, g, b]";
	CommandDispatcher->BindCommand(TEXT("vset /object/[str]/mobility [str]"), Cmd, Help);

	Cmd = FDispatcherDelegate::CreateRaw(this, &FObjectCommandHandler::GetObjectName);
	Help = "[debug] Get the object name";
	CommandDispatcher->BindCommand(TEXT("vget /object/[str]/name"), Cmd, Help);

	Cmd = FDispatcherDelegate::CreateRaw(this, &FObjectCommandHandler::GetObjectBoundingBox);
	Help = "[debug] Get the object name";
	CommandDispatcher->BindCommand(TEXT("vget /object/[str]/bounding_box"), Cmd, Help);

	Cmd = FDispatcherDelegate::CreateRaw(this, &FObjectCommandHandler::GetObjectGID);
	Help = "Get the global id of an object";
	CommandDispatcher->BindCommand(TEXT("vget /object/[str]/gid"), Cmd, Help);

	Cmd = FDispatcherDelegate::CreateRaw(this, &FObjectCommandHandler::GetObjectLocation);
	Help = "Get object location [x, y, z]";
	CommandDispatcher->BindCommand(TEXT("vget /object/[str]/location"), Cmd, Help);

	Cmd = FDispatcherDelegate::CreateRaw(this, &FObjectCommandHandler::GetObjectRotation);
	Help = "Get object rotation [pitch, yaw, roll]";
	CommandDispatcher->BindCommand(TEXT("vget /object/[str]/rotation"), Cmd, Help);

	Cmd = FDispatcherDelegate::CreateRaw(this, &FObjectCommandHandler::SetObjectLocation);
	Help = "Set object location [x, y, z]";
	CommandDispatcher->BindCommand(TEXT("vset /object/[str]/location [float] [float] [float]"), Cmd, Help);

	Cmd = FDispatcherDelegate::CreateRaw(this, &FObjectCommandHandler::SetObjectRotation);
	Help = "Set object rotation [pitch, yaw, roll]";
	CommandDispatcher->BindCommand(TEXT("vset /object/[str]/rotation [float] [float] [float]"), Cmd, Help);


}
FExecStatus FObjectCommandHandler::HelloWorld(const TArray<FString>& Args)
{
	FString Msg;
	FSHA1 SHATest;
	uint8 Hash[FSHA1::DigestSize];
	std::string Test= "abc";
	SHATest.Update((uint8*)Test.c_str(), Test.size());
	SHATest.Final();
	SHATest.GetHash(Hash);
	Msg = BytesToHex(Hash, FSHA1::DigestSize).ToLower();
	//Temporary romove for now for checking adapability
	//GID = GID.Left(8);
	return FExecStatus::OK(Msg);
	//Msg += "Hello World!";
	return FExecStatus::OK(Msg);
	//APointLight *pointlight = Cast<APointLight> (GE
}
FExecStatus FObjectCommandHandler::GetObjects(const TArray<FString>& Args)
{
	return FObjectPainter::Get().GetObjectList();
}

FExecStatus FObjectCommandHandler::SetObjectColor(const TArray<FString>& Args)
{
	// ObjectName, R, G, B, A = 255
	// The color format is RGBA
	if (Args.Num() == 4)
	{
		FString ObjectName = Args[0];
		uint32 R = FCString::Atoi(*Args[1]), G = FCString::Atoi(*Args[2]), B = FCString::Atoi(*Args[3]), A = 255; // A = FCString::Atoi(*Args[4]);
		FColor NewColor(R, G, B, A);

		return FObjectPainter::Get().SetActorColor(ObjectName, NewColor);
	}

	return FExecStatus::InvalidArgument;
}
FExecStatus FObjectCommandHandler::SetObjectMoBility(const TArray<FString>& Args)
{
	// ObjectName, R, G, B, A = 255
	// The color format is RGBA
	if (Args.Num() == 2)
	{
		FString ObjectName = Args[0];
		FString Option = Args[1];
		AActor* Object = FObjectPainter::Get().GetObject(ObjectName);
		if (Object == NULL)
		{
			return FExecStatus::Error(FString::Printf(TEXT("Can not find object %s"), *ObjectName));
		}
		TArray<UMeshComponent*> PaintableComponents;
		Object->GetComponents<UMeshComponent>(PaintableComponents);
		for (auto MeshComponent : PaintableComponents)
		{
			if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(MeshComponent))
			{
				if (Option.ToLower() == "movable")
				{
					StaticMeshComponent->Mobility = EComponentMobility::Movable;
				}
				else if (Option.ToLower() == "static")
				{
					StaticMeshComponent->Mobility = EComponentMobility::Static;
				}
				else if (Option.ToLower() == "stationary")
				{
					StaticMeshComponent->Mobility = EComponentMobility::Stationary;
				}
				else
				{
					return FExecStatus::InvalidArgument;
				}
				return FExecStatus::OK(FString("Mobility set to: ") + Option);
			}
		}
	}

	return FExecStatus::InvalidArgument;
}

FExecStatus FObjectCommandHandler::GetObjectColor(const TArray<FString>& Args)
{
	if (Args.Num() == 1)
	{
		FString ObjectName = Args[0];
		return FObjectPainter::Get().GetActorColor(ObjectName);
	}

	return FExecStatus::InvalidArgument;
}

FExecStatus FObjectCommandHandler::GetObjectName(const TArray<FString>& Args)
{
	if (Args.Num() == 1)
	{
		return FExecStatus::OK(Args[0]);
	}
	return FExecStatus::InvalidArgument;
}

FExecStatus FObjectCommandHandler::GetObjectBoundingBox(const TArray<FString>& Args)
{
	if (Args.Num() == 1)
	{
		FString ObjectName = Args[0];
		AActor* Object = FObjectPainter::Get().GetObject(ObjectName);
		if (Object == NULL)
		{
			return FExecStatus::Error(FString::Printf(TEXT("Can not find object %s"), *ObjectName));
		}
		// Store the information of the bounding box
		float minX = (float)INT_MAX;
		float minY = (float)INT_MAX;
		float minZ = (float)INT_MAX;
		float maxX = (float)INT_MIN;
		float maxY = (float)INT_MIN;
		float maxZ = (float)INT_MIN;

		TArray<UMeshComponent*> PaintableComponents;
		Object->GetComponents<UMeshComponent>(PaintableComponents);
		for (auto MeshComponent : PaintableComponents)
		{
			if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(MeshComponent))
			{
				UStaticMesh* StaticMesh;
#if ENGINE_MINOR_VERSION >= 14  // Assume major version is 4
				StaticMesh = StaticMeshComponent->GetStaticMesh(); // This is a new function introduced in 4.14
#else
				StaticMesh = StaticMeshComponent->StaticMesh; // This is deprecated in 4.14, add here for backward compatibility
#endif
				if (StaticMesh)
				{
					uint32 NumLODLevel = StaticMesh->RenderData->LODResources.Num();
					for (uint32 PaintingMeshLODIndex = 0; PaintingMeshLODIndex < NumLODLevel; PaintingMeshLODIndex++)
					//if (NumLODLevel > 0)
					{

						FPositionVertexBuffer *VertexBuffer = &StaticMesh->RenderData->LODResources[PaintingMeshLODIndex].PositionVertexBuffer;
						const int32 VertexCount = VertexBuffer->GetNumVertices();
						for (int32 Index = 0; Index < VertexCount; Index++)
						{
							//World Coordinate
							const FVector WorldSpaceVertexLocation = Object->GetActorLocation() + Object->GetTransform().TransformVector(VertexBuffer->VertexPosition(Index));
							minX = fminf(minX, WorldSpaceVertexLocation.X);
							minY = fminf(minY, WorldSpaceVertexLocation.Y);
							minZ = fminf(minZ, WorldSpaceVertexLocation.Z);
							maxX = fmaxf(maxX, WorldSpaceVertexLocation.X);
							maxY = fmaxf(maxY, WorldSpaceVertexLocation.Y);
							maxZ = fmaxf(maxZ, WorldSpaceVertexLocation.Z);
						}

					}
				}
			}
		}
		//FString Output = "Min: " + FString::SanitizeFloat(minX) + " " + FString::SanitizeFloat(minY) + " " + FString::SanitizeFloat(minZ) + " " +
		//	             "Max: " + FString::SanitizeFloat(maxX) + " " + FString::SanitizeFloat(maxY) + " " + FString::SanitizeFloat(maxZ);
		return FExecStatus::OK(FString::Printf(TEXT("Min: %.2f %.2f %.2f  Max:  %.2f %.2f %.2f"), minX, minY, minZ, maxX, maxY, maxZ));
	}
	return FExecStatus::InvalidArgument;
}

FExecStatus FObjectCommandHandler::GetObjectGID(const TArray<FString>& Args)

{
// Save information to file
	if (Args.Num() == 1)
	{
		FSHA1 HashState;
		uint8 Hash[FSHA1::DigestSize];
		FString GID;
		FString ObjectName = Args[0];
		AActor* Object = FObjectPainter::Get().GetObject(ObjectName);
		if (Object == NULL)
		{
			return FExecStatus::Error(FString::Printf(TEXT("Can not find object %s"), *ObjectName));
		}
//Save information to file
		FString SaveDirectory = FString("Objectdata/");
		FString FileName = FString(ObjectName+".sav");
		FString TextToSave = FString("");
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		TArray<UMeshComponent*> PaintableComponents;
        Object->GetComponents<UMeshComponent>(PaintableComponents);
		for (auto MeshComponent : PaintableComponents)
		{
			if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(MeshComponent))
			{
				UStaticMesh* StaticMesh;
#if ENGINE_MINOR_VERSION >= 14  // Assume major version is 4
				StaticMesh = StaticMeshComponent->GetStaticMesh(); // This is a new function introduced in 4.14
#else
				StaticMesh = StaticMeshComponent->StaticMesh; // This is deprecated in 4.14, add here for backward compatibility
#endif
				
                /*
				//Hash the Mobility of staticmeshcomponent
				std::string HashMobility;
				if (StaticMeshComponent->Mobility == EComponentMobility::Movable)
				{
					HashMobility = "1";
				}
				else if (StaticMeshComponent->Mobility == EComponentMobility::Static)
				{
					HashMobility = "2";
				}
				else
				{
					HashMobility = "3";
				}
				HashState.Update((uint8*)HashMobility.c_str(), HashMobility.size());
				*/
				/*
				//Hash the Material (Name) of staticmeshcomponent
				
				for (int Materialindex = 0; Materialindex < StaticMeshComponent->GetNumMaterials(); Materialindex++)
				{
					//UMaterialInstanceDynamic * Material = UMaterialInstanceDynamic::Create(StaticMeshComponent->GetMaterial(Materialindex),nullptr);
					//FString MaterialName = Material->GetName();
					FString MaterialName = StaticMeshComponent->GetMaterial(Materialindex)->GetName();
					//std::string MaterialConvert(TCHAR_TO_UTF8(*MaterialName));
					//HashState.Update((uint8*)MaterialConvert.c_str(), MaterialConvert.size());
				}
				*/
				//Hash the Vertex positions
				if (StaticMesh)
				{
					uint32 NumLODLevel = StaticMesh->RenderData->LODResources.Num();
					for (uint32 PaintingMeshLODIndex = 0; PaintingMeshLODIndex < NumLODLevel; PaintingMeshLODIndex++)
					//if (NumLODLevel > 0)
					{

						FPositionVertexBuffer *VertexBuffer = &StaticMesh->RenderData->LODResources[PaintingMeshLODIndex].PositionVertexBuffer;
						const int32 VertexCount = VertexBuffer->GetNumVertices();
						for (int32 Index = 0; Index < VertexCount; Index++)
						{
							//World Coordinate
							const FVector WorldSpaceVertexLocation = Object->GetActorLocation() + Object->GetTransform().TransformVector(VertexBuffer->VertexPosition(Index));
							//Local Coordinate
							//const FVector LocalLocation = Object->GetTransform().TransformVector(VertexBuffer->VertexPosition(Index));
							const FVector VPosition = VertexBuffer->VertexPosition(Index);
							FString Combine = FString::FromInt(int(100 * VPosition.X)) + FString::FromInt(int(100 * VPosition.Y)) + FString::FromInt(int(100 * VPosition.Z));
							TextToSave += FString::FromInt(int(100 * VPosition.X)) +" "+ FString::FromInt(int(100 * VPosition.Y)) + " "+FString::FromInt(int(100 * VPosition.Z)) +"\n";
							std::string Convertstring(TCHAR_TO_UTF8(*Combine));
							HashState.Update((uint8*)Convertstring.c_str(), Convertstring.size());
						}

					}					
				}
			}
		}
		HashState.Final();
		HashState.GetHash(Hash);
		if (PlatformFile.CreateDirectoryTree(*SaveDirectory))
		{
			// Get absolute file path
			FString AbsoluteFilePath = SaveDirectory + "/" + FileName;
			FFileHelper::SaveStringToFile(TextToSave, *AbsoluteFilePath);
		}
		GID =BytesToHex(Hash, FSHA1::DigestSize).ToLower();
		//Temporary romove for now for checking adapability
		//GID = GID.Left(8);
		return FExecStatus::OK(GID);
	}
	return FExecStatus::InvalidArgument;
}

FExecStatus FObjectCommandHandler::CurrentObjectHandler(const TArray<FString>& Args)
{
	// At least one parameter
	if (Args.Num() >= 2)
	{
		FString Uri = "";
		// Get the name of current object
		FHitResult HitResult;
		// The original version for the shooting game use CameraComponent
		APawn* Pawn = FUE4CVServer::Get().GetPawn();
		FVector StartLocation = Pawn->GetActorLocation();
		// FRotator Direction = GetActorRotation();
		FRotator Direction = Pawn->GetControlRotation();

		FVector EndLocation = StartLocation + Direction.Vector() * 10000;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Pawn);

		APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
		if (PlayerController != nullptr)
		{
			FHitResult TraceResult(ForceInit);
			PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, false, TraceResult);
			FString TraceString;
			if (TraceResult.GetActor() != nullptr)
			{
				TraceString += FString::Printf(TEXT("Trace Actor %s."), *TraceResult.GetActor()->GetName());
			}
			if (TraceResult.GetComponent() != nullptr)
			{
				TraceString += FString::Printf(TEXT("Trace Comp %s."), *TraceResult.GetComponent()->GetName());
			}
			// TheHud->TraceResultText = TraceString;
			// Character->ConsoleOutputDevice->Log(TraceString);
		}
		// TODO: This is not working well.

		if (GWorld->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
		{
			AActor* HitActor = HitResult.GetActor();

			// UE_LOG(LogUnrealCV, Warning, TEXT("%s"), *HitActor->GetActorLabel());
			// Draw a bounding box of the hitted object and also output the name of it.
			FString ActorName = HitActor->GetHumanReadableName();
			FString Method = Args[0], Property = Args[1];
			Uri = FString::Printf(TEXT("%s /object/%s/%s"), *Method, *ActorName, *Property); // Method name

			for (int32 ArgIndex = 2; ArgIndex < Args.Num(); ArgIndex++) // Vargs
			{
				Uri += " " + Args[ArgIndex];
			}
			FExecStatus ExecStatus = CommandDispatcher->Exec(Uri);
			return ExecStatus;
		}
		else
		{
			return FExecStatus::Error("Can not find current object");
		}
	}
	return FExecStatus::InvalidArgument;
}

FExecStatus FObjectCommandHandler::GetObjectLocation(const TArray<FString>& Args)
{
	if (Args.Num() == 1)
	{
		FString ObjectName = Args[0];
		AActor* Object = FObjectPainter::Get().GetObject(ObjectName);
		if (Object == NULL)
		{
			return FExecStatus::Error(FString::Printf(TEXT("Can not find object %s"), *ObjectName));
		}

		FVector Location = Object->GetActorLocation();
		return FExecStatus::OK(FString::Printf(TEXT("%.2f %.2f %.2f"), Location.X, Location.Y, Location.Z));
	}

	return FExecStatus::InvalidArgument;
}

FExecStatus FObjectCommandHandler::GetObjectRotation(const TArray<FString>& Args)
{
	if (Args.Num() == 1)
	{
		FString ObjectName = Args[0];
		AActor* Object = FObjectPainter::Get().GetObject(ObjectName);
		if (Object == NULL)
		{
			return FExecStatus::Error(FString::Printf(TEXT("Can not find object %s"), *ObjectName));
		}

		// TODO: support quaternion
		FRotator Rotation = Object->GetActorRotation();
		return FExecStatus::OK(FString::Printf(TEXT("%.2f %.2f %.2f"), Rotation.Pitch, Rotation.Yaw, Rotation.Roll));
	}
	return FExecStatus::InvalidArgument;
}

/** There is no guarantee this will always succeed, for example, hitting a wall */
FExecStatus FObjectCommandHandler::SetObjectLocation(const TArray<FString>& Args)
{
	if (Args.Num() == 4)
	{
		FString ObjectName = Args[0];
		AActor* Object = FObjectPainter::Get().GetObject(ObjectName);
		if (Object == NULL)
		{
			return FExecStatus::Error(FString::Printf(TEXT("Can not find object %s"), *ObjectName));
		}

		// TODO: Check whether this object is movable
		float X = FCString::Atof(*Args[1]), Y = FCString::Atof(*Args[2]), Z = FCString::Atof(*Args[3]);
		FVector NewLocation = FVector(X, Y, Z);
		bool Success = Object->SetActorLocation(NewLocation);

		return FExecStatus::OK();
	}
	return FExecStatus::InvalidArgument;
}

FExecStatus FObjectCommandHandler::SetObjectRotation(const TArray<FString>& Args)
{
	if (Args.Num() == 4)
	{
		FString ObjectName = Args[0];
		AActor* Object = FObjectPainter::Get().GetObject(ObjectName);
		if (Object == NULL)
		{
			return FExecStatus::Error(FString::Printf(TEXT("Can not find object %s"), *ObjectName));
		}

		// TODO: Check whether this object is movable
		float Pitch = FCString::Atof(*Args[1]), Yaw = FCString::Atof(*Args[2]), Roll = FCString::Atof(*Args[3]);
		FRotator Rotator = FRotator(Pitch, Yaw, Roll);
		bool Success = Object->SetActorRotation(Rotator);
		return FExecStatus::OK();
	}
	return FExecStatus::InvalidArgument;
}

