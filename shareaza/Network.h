//
// Network.h
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

#include "ThreadImpl.h"

class CNeighbour;
class CBuffer;
class CPacket;
class CG2Packet;
class CRouteCache;
class CQueryKeys;
class CQuerySearch;
class CQueryHit;

enum // It is used from CNetwork::IsFirewalled
{
	CHECK_BOTH, CHECK_TCP, CHECK_UDP
};

class CNetwork :
	public CThreadImpl
{
// Construction
public:
	CNetwork();
	virtual ~CNetwork();

// Attributes
public:
	CRouteCache*	NodeRoute;
	CRouteCache*	QueryRoute;
	CQueryKeys*		QueryKeys;

	CMutex			m_pSection;
	SOCKADDR_IN		m_pHost;				// Structure (Windows Sockets) which holds address of the local machine
	BOOL			m_bAutoConnect;
	BOOL			m_bTCPListeningReady;
	BOOL			m_bUDPListeningReady;
	DWORD			m_tStartedConnecting;	// The time Shareaza started trying to connect
	DWORD			m_tLastConnect;			// The last time a neighbour connection attempt was made
	DWORD			m_tLastED2KServerHop;	// The last time the ed2k server was changed
protected:
	CStringA		m_sHostName;
	CList< ULONG >	m_pHostAddresses;
	DWORD			m_nSequence;
	struct ResolveStruct
	{
		CString* m_sAddress;
		PROTOCOLID m_nProtocol;
		WORD m_nPort;
		BYTE m_nCommand;
		union
		{
			char m_pBuffer[ MAXGETHOSTSTRUCT ];
			HOSTENT m_pHost;
		};
	};
	CMap< HANDLE, HANDLE, ResolveStruct*, ResolveStruct* > m_pLookups;

	void		OnRun();

// Operations
public:
	BOOL		IsSelfIP(IN_ADDR nAddress) const;
	BOOL		IsAvailable() const;
	bool		IsConnected() const;
	bool		IsListening() const;
	bool		IsWellConnected() const;
	bool		IsStable() const;
	BOOL		IsFirewalled(int nCheck = CHECK_UDP) const;
	DWORD		GetStableTime() const;
	BOOL		IsConnectedTo(IN_ADDR* pAddress) const;
	BOOL		ReadyToTransfer(DWORD tNow) const;		// Are we ready to start downloading?
public:
	BOOL		Connect(BOOL bAutoConnect = FALSE);
	void		Disconnect();
	BOOL		ConnectTo(LPCTSTR pszAddress, int nPort = 0, PROTOCOLID nProtocol = PROTOCOL_NULL, BOOL bNoUltraPeer = FALSE);
	void		AcquireLocalAddress(LPCTSTR pszHeader);
	BOOL		Resolve(LPCTSTR pszHost, int nPort, SOCKADDR_IN* pHost, BOOL bNames = TRUE) const;
	BOOL		AsyncResolve(LPCTSTR pszAddress, WORD nPort, PROTOCOLID nProtocol, BYTE nCommand);
	BOOL		IsReserved(IN_ADDR* pAddress, bool bCheckLocal=true);
	WORD		RandomPort() const;
	void		CreateID(Hashes::Guid& oID);
	BOOL		IsFirewalledAddress(LPVOID pAddress, BOOL bIncludeSelf = FALSE, BOOL bForceCheck = FALSE );
public:
	BOOL		GetNodeRoute(const Hashes::Guid& oGUID, CNeighbour** ppNeighbour, SOCKADDR_IN* pEndpoint);
	BOOL		RoutePacket(CG2Packet* pPacket);
	BOOL		SendPush(const Hashes::Guid& oGUID, DWORD nIndex = 0);
	BOOL		RouteHits(CQueryHit* pHits, CPacket* pPacket);
	void		OnWinsock(WPARAM wParam, LPARAM lParam);
	void		OnQuerySearch(CQuerySearch* pSearch);
	void		OnQueryHits(CQueryHit* pHits);
public:
	void		UDPHostCache(IN_ADDR* pAddress, WORD nPort);
	void		UDPKnownHubCache(IN_ADDR* pAddress, WORD nPort);

	friend class CHandshakes;
	friend class CNeighbours;
};

extern CNetwork Network;
