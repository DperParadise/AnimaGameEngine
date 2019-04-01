#include "Animation.h"
#include <assimp/anim.h>
#include "Skeleton.h"
#include "Globals.h"
#include <algorithm>
#include <cmath>

KeyFrame::KeyFrame()
{
	KeyFrame(glm::quat(), glm::vec3(0.0f), glm::vec3(1.0f), 0.0);
}

KeyFrame::KeyFrame(glm::quat rot, glm::vec3 pos, glm::vec3 scale, float time) : rotation(rot), position(pos), scale(scale), timeInTicks(time) {}

BoneChannel::BoneChannel(const std::string & boneName) : boneName(boneName){}

Animation::Animation(const std::string & name, aiAnimation *aiAnimation, Skeleton *skeleton) : name(name), skeleton(skeleton)
{
	Load(aiAnimation);
	SortBoneChannelsByBoneId();
	PrintBones();
}

Animation::~Animation()
{
	for (BoneChannel *bc : animation)
	{
		RELEASE(bc)
	}
}

void Animation::Update(float dt)
{
	if (!isPlaying)
		return;
	
	accumTime += dt;

	if (accumTime > durationInSecs) 
	{
		if (loop)
			accumTime -= durationInSecs;
		else
			accumTime = durationInSecs;
	}

	float tick = accumTime * ticksPerSecond;
	float intTick = 0.0f;
	float interpolationFactor = modff(tick, &intTick);

	for (int i = 0; i < animation.size(); ++i)
	{
		BoneChannel *boneChannel = animation[i];
		KeyFrame &previous = boneChannel->keyFrames[intTick];
		KeyFrame &next = boneChannel->keyFrames[(int)(intTick + 1) % durationInTicks];

		glm::quat newRot = glm::mix(previous.rotation, next.rotation, interpolationFactor);
		newRot = glm::normalize(newRot);
		glm::vec3 newPos = glm::mix(previous.position, next.position, interpolationFactor);
		glm::vec3 newScale = glm::mix(previous.scale, next.scale, interpolationFactor);

		skeleton->skeleton[i].localPose.rotation = newRot;
		skeleton->skeleton[i].localPose.position = newPos;
		skeleton->skeleton[i].localPose.scale = newScale;
	}
}

void Animation::Load(aiAnimation *aiAnimation)
{
	ticksPerSecond = aiAnimation->mTicksPerSecond != 0 ? aiAnimation->mTicksPerSecond : ticksPerSecondDefault;
	durationInTicks = (float)aiAnimation->mDuration;
	durationInSecs = (float)aiAnimation->mDuration / ticksPerSecond;
	animation.reserve(aiAnimation->mNumChannels);

	for (int i = 0; i < aiAnimation->mNumChannels; ++i)
	{
		aiNodeAnim *nodeAnim = aiAnimation->mChannels[i];
		BoneChannel *boneChannel = new BoneChannel(nodeAnim->mNodeName.C_Str());
		int boneId = skeleton->FindBoneInSkeleton(boneChannel->boneName);
		if (boneId == -1)
		{
			MYLOG("Error: \"%s\" not found in the skeleton", boneChannel->boneName.c_str())
			return;
		}
		boneChannel->boneId = boneId;

		boneChannel->keyFrames.reserve(nodeAnim->mNumPositionKeys);
		for (int j = 0; j < nodeAnim->mNumPositionKeys; ++j)
		{
			aiQuaternion rot = nodeAnim->mRotationKeys[j].mValue;
			aiVector3D pos = nodeAnim->mPositionKeys[j].mValue;

			glm::quat rotGLM(rot.w, rot.x, rot.y, rot.z);
			glm::vec3 posGLM(pos.x, pos.y, pos.z);

			KeyFrame keyFrame(rotGLM, posGLM, glm::vec3(1.0f), (float)nodeAnim->mRotationKeys[j].mTime);

			boneChannel->keyFrames.push_back(keyFrame);
		}

		//In case the number of scaling keys is not the same as rotation and position keys
		for (int j = 0; j < nodeAnim->mNumScalingKeys; ++j)
		{
			aiVector3D scale = nodeAnim->mScalingKeys[j].mValue;
			boneChannel->keyFrames[j].scale = glm::vec3(scale.x, scale.y, scale.z);
		}

		animation.push_back(boneChannel);
	}
}

void Animation::Play(float startTime, bool loop)
{
	isPlaying = true;
	this->loop = loop;
}

void Animation::Stop()
{
	isPlaying = false;
}

void Animation::SortBoneChannelsByBoneId()
{
	std::sort(animation.begin(), animation.end(), Less);
}

bool Animation::Less(const BoneChannel *left, const BoneChannel *right)
{
	return left->boneId < right->boneId;
}

void Animation::PrintBones()
{
	MYLOG("======= start bone channels ======")
	for (BoneChannel *bc : animation)
	{
		MYLOG("bone:%s id:%d", bc->boneName.c_str(), bc->boneId)
	}
	MYLOG("======= end bone channels ======")
}

