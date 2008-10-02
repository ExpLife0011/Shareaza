//
// MD5.h
//
// Copyright (c) Shareaza Development Team, 2002-2008.
// This file is part of SHAREAZA (shareaza.sourceforge.net)
//
// Shareaza is free software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//
// Shareaza is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shareaza; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#pragma once

class HASHLIB_API CMD5
{
public:
	CMD5();
	void Reset();
	void Add(const void* pData, size_t nLength);
	void Finish();

	struct HASHLIB_API Digest // 128 bit
	{
		uint32& operator[](size_t i) { return data[ i ]; }
		const uint32& operator[](size_t i) const { return data[ i ]; }
		uint32 data[ 4 ];
	};

	template< typename T >
	inline void GetHash(T& oHash) const
	{
		std::transform( m_State.m_nState,
			m_State.m_nState + sizeof( m_State.m_nState ) / sizeof( m_State.m_nState[ 0 ] ),
			&oHash[ 0 ], transformToLE< uint32 > );
	}

private:
	struct MD5State
	{
		static const size_t blockSize = 64;
		uint64	m_nCount;
		uint32	m_nState[ 4 ];
		uchar	m_oBuffer[ blockSize ];
	};
	MD5State m_State;

#ifndef HASHLIB_USE_ASM
	void	Transform(const uint32* data);
#endif
};
