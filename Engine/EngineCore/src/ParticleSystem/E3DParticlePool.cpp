#include "E3DParticlePool.h"
#include "E3DParticle.h"
#include "../Source/E3DDebug.h"

namespace E3DEngine
{
	void ParticlePool::CreateParticlePool(UINT particleNumber)
	{
		for (int i = 0; i < particleNumber; i++)
		{
			Particle *p = new Particle(); 
			if (p == nullptr)
			{
				Debug::Log(ell_Error, "CreateParticlePool memery deplete!");
				assert(false);
			}
			share_pointer<Particle> sp(p);
			m_ParticleList.emplace_back(sp);
		}
		if (particleNumber > m_nMaxParticleNumber)
		{
			m_nMaxParticleNumber = particleNumber;
		}
		Debug::Log(ell_Warning, "particle pool size = %d", m_ParticleList.size());
	}

	share_pointer<Particle>  ParticlePool::ActiveParticleFromPool()
	{
		if (m_ParticleList.empty())
		{
			CreateParticlePool(1);
		}
		share_pointer<Particle> _p = m_ParticleList.front();
		m_ParticleList.pop_front();
		return _p;
	}

	void ParticlePool::DeleteParticleToDeathPool(share_pointer<Particle> particle)
	{
		m_ParticleList.emplace_back(particle);
		if (m_ParticleList.size() > m_nMaxParticleNumber)
		{
			int needRemoveSize = m_ParticleList.size() - m_nMaxParticleNumber;
			int hasremoveSize = 0;
			// ɾ������Ĳ��֣���֤�������ӳع���
			for (particleListIterator itr = m_ParticleList.begin(); itr != m_ParticleList.end();)
			{
				itr = m_ParticleList.erase(itr);
				hasremoveSize ++;
				if (hasremoveSize == needRemoveSize)
				{
					break;
				}

			}
		}
	}

	void ParticlePool::DeleteParticleToDeathPool(std::list<share_pointer<Particle>>& particlePools)
	{
		m_ParticleList.splice(m_ParticleList.end(), particlePools);
		Debug::Log(ell_Warning, "==>particle pool size = %d", m_ParticleList.size());
	}
}
