#include "sound_manager.h"

const std::string	CSoundManager::m_file_path[(int)SOUND_ID::MAX] =
{
	"data\\sound\\title.wav",
	"data\\sound\\gamemain_bgm.wav",
	"data\\sound\\thread_attack.wav",
	"data\\sound\\enemy_thread_damage.wav",
	"data\\sound\\subsist.wav",
	"data\\sound\\select_change.wav",
	"data\\sound\\click.wav",
	"data\\sound\\charge.wav",
	"data\\sound\\rush.wav",
	"data\\sound\\buff_upgrade.wav",
	"data\\sound\\player_damage.wav",
	"data\\sound\\enemy_rush_damage.wav", 
	"data\\sound\\result.wav"

};

CSoundManager& CSoundManager::GetInstance(void)
{
	static CSoundManager instance;
	return instance;
}

void CSoundManager::Initialize(void)
{
	//デフォルトの音量
	m_SetVolume = 9000;

	for (int i = 0; i < (int)SOUND_ID::MAX; ++i)
	{
		vivid::LoadSound(m_file_path[i]);
		SetVolume((SOUND_ID)i, m_SetVolume);
	}


}

void CSoundManager::Finalize(void)
{

}


void CSoundManager::Play(SOUND_ID id, bool loop)
{
	vivid::PlaySound(m_file_path[(int)id], loop);
}

void CSoundManager::Stop(SOUND_ID id)
{
	vivid::StopSound(m_file_path[(int)id]);
}

void CSoundManager::SetVolume(SOUND_ID id, int volume)
{
	vivid::SetSoundVolume(m_file_path[(int)id], volume);
}


/*
 *  コンストラクタ
 */
CSoundManager::
CSoundManager(void)
{
}

/*
 *  コピーコンストラクタ
 */
CSoundManager::
CSoundManager(const CSoundManager& rhs)
{
	(void)rhs;
}

/*
 *  デストラクタ
 */
CSoundManager::
~CSoundManager(void)
{
}

/*
 *  代入演算子
 */
CSoundManager&
CSoundManager::
operator=(const CSoundManager& rhs)
{
	(void)rhs;

	return *this;
}
