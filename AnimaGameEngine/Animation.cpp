#include "Animation.h"
#include <assimp/anim.h>
#include "Skeleton.h"
#include "Globals.h"
#include <algorithm>

KeyFrame::KeyFrame(glm::quat rot, glm::vec3 pos, glm::vec3 scale, float time) : rotation(rot), position(pos), scale(scale), time(time) {}

BoneChannel::BoneChannel(const std::string & boneName) : boneName(boneName){}

Animation::Animation(const std::string & name, aiAnimation *aiAnimation, const Skeleton *skeleton)
{
	Load(aiAnimation, skeleton);
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

void Animation::Load(aiAnimation *aiAnimation, const Skeleton *skeleton)
{
	ticksPerSecond = aiAnimation->mTicksPerSecond != 0 ? aiAnimation->mTicksPerSecond : ticksPerSecondDefault;
	duration = (float)aiAnimation->mDuration / ticksPerSecond;
	animation.reserve(aiAnimation->mNumChannels);

	for (int i = 0; i < aiAnimation->mNumChannels; ++i)
	{
		aiNodeAnim *nodeAnim = aiAnimation->mChannels[i];
		BoneChannel *boneChannel = new BoneChannel(nodeAnim->mNodeName.C_Str());
		int boneId = skeleton->FindBoneInSkeleton(boneChannel->boneName);
		if (boneId == -1)
		{
			MYLOG("Error: %s not found in the skeleton", boneChannel->boneName)
			return;
		}
		boneChannel->boneId = boneId;

		boneChannel->keyFrames.reserve(nodeAnim->mNumPositionKeys);
		for (int j = 0; j < nodeAnim->mNumPositionKeys; ++j)
		{
			aiQuaternion rot = nodeAnim->mRotationKeys[j].mValue;
			aiVector3D pos = nodeAnim->mPositionKeys[j].mValue;
			aiVector3D scale = nodeAnim->mScalingKeys[j].mValue;

			glm::quat rotGLM(rot.w, rot.x, rot.y, rot.z);
			glm::vec3 posGLM(pos.x, pos.y, pos.z);
			glm::vec3 scaleGLM(scale.x, scale.y, scale.z);

			float time = (float)nodeAnim->mRotationKeys[j].mTime / ticksPerSecond;
			KeyFrame keyFrame(rotGLM, posGLM, scaleGLM, (float)nodeAnim->mRotationKeys[j].mTime);

			boneChannel->keyFrames.push_back(keyFrame);
		}
		animation.push_back(boneChannel);
	}
}

void Animation::Play(float startTime, bool loop)
{
}

void Animation::Stop()
{
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

