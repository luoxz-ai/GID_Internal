#pragma once
#include "CommandDispatcher.h"

class FObjectCommandHandler : public FCommandHandler
{
public:
	FObjectCommandHandler(FCommandDispatcher* InCommandDispatcher) : FCommandHandler(InCommandDispatcher)
	{}
	void RegisterCommands();

	FExecStatus HelloWorld(const TArray<FString>& Args);
	/** Get a list of all objects in the scene */
	FExecStatus GetObjects(const TArray<FString>& Args);
	/** Get the annotation color of an object (Notice: not the appearance color) */
	FExecStatus GetObjectColor(const TArray<FString>& Args);
	/** Set the annotation color of an object */
	FExecStatus SetObjectColor(const TArray<FString>& Args);
	/** Get the name of an object */
	FExecStatus GetObjectName(const TArray<FString>& Args);
	/** Get the bounding box of an object */
	FExecStatus GetObjectBoundingBox(const TArray<FString>& Args);
	/** Get the global ID of an object */
	FExecStatus GetObjectGID(const TArray<FString>& Args);

	FExecStatus CurrentObjectHandler(const TArray<FString>& Args);

	/** Get object location */
	FExecStatus GetObjectLocation(const TArray<FString>& Args);

	/** Get object rotation */
	FExecStatus GetObjectRotation(const TArray<FString>& Args);

	/** Set object location */
	FExecStatus SetObjectLocation(const TArray<FString>& Args);

	/** Set object rotation */
	FExecStatus SetObjectRotation(const TArray<FString>& Args);


};
