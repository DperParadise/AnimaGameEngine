#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct aiAnimation;
class Skeleton;

struct KeyFrame
{
	KeyFrame(glm::quat rot, glm::vec3 pos, glm::vec3 scale, float time);
	glm::quat rotation;
	glm::vec3 position;
	glm::vec3 scale;
	float time;
};

struct BoneChannel
{
	BoneChannel(const std::string &boneName);
	
	std::vector<KeyFrame> keyFrames;
	unsigned int boneId;
	std::string boneName;
};


class Animation
{
public:
	Animation(const std::string &name, aiAnimation *aiAnimation, const Skeleton *skeleton);
	~Animation();
	std::vector<BoneChannel*> animation;
	float startTime = 0.0f;
	float currentTime = 0.0f;
	float duration = 0.0f;
	float ticksPerSecond = 0.0;
	std::string name;

private:
	double ticksPerSecondDefault = 25.0f;
	bool loop = false;

	void Load(aiAnimation *aiAnimation, const Skeleton *skeleton);
	void Play(float startTime, bool loop);
	void Stop();
	void SortBoneChannelsByBoneId();
	static bool Less(const BoneChannel *left, const BoneChannel *right);

	void PrintBones();
};

#endif // !_ANIMATION_H_

