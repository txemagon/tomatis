/*
 * Copyright (c) 2001-2004 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __SIGNALV3IMPLSERIOUS__
#define __SIGNALV3IMPLSERIOUS__

#ifndef __SIGNALV3__
#error "This is an internal implementation header. You are not allowed to include it directly!"
#endif

#include "ConnectionHandler.hxx"

namespace SigSlot
{

template < typename ParmType1, typename ParmType2, typename ParmType3 >
	class Signalv3 : public Signal
{
public:
	typedef typename CBL::Functor3<ParmType1,ParmType2, ParmType3 >              tCallbackType;

public:
	
	virtual ~Signalv3()
	{
		mSuper.DestroyConnections();
	}

	void Connect( Slotv3<ParmType1, ParmType2, ParmType3>& slot )
	{
		Connection c( AssignConnection(), this );

		mSuper.AddCallback( c.GetID(), &slot, slot.GetMethod() );

		slot.Bind(c);
	}
	
	void Emit( ParmType1 parm1, ParmType2 parm2, ParmType3 parm3 )
	{
		if ( mSuper.HasNoCallbacks() )
			return;
		
		typename tSuperType::tCallList calls = mSuper.GetCalls();
		typename tSuperType::tCallIterator i = calls.begin();
		typename tSuperType::tCallIterator end = calls.end();

		while ( i != end )
			{
				(*(*i))( parm1, parm2, parm3 );
				i++;
			}
		
	}

	void FreeConnection( Connection* pConnection )
	{
		mSuper.RemoveCall( pConnection->GetID() );
		FreeConnectionId( pConnection->GetID() );
	}
private:
	typedef Signalv3<ParmType1,ParmType2,ParmType3>                tSignalType;
	typedef ConnectionHandler<tSignalType >     tSuperType;

	tSuperType  mSuper;
};

}


#endif // Signalv2ImplSerious.hxx

