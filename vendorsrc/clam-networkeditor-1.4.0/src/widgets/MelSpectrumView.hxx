/*
 * Copyright (c) 2001-2006 MUSIC TECHNOLOGY GROUP (MTG)
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

#ifndef MelSpectrumView_hxx
#define MelSpectrumView_hxx

#include "FloatArrayDataSource.hxx"

#include <QtDesigner/QDesignerExportWidget>
#include "BarGraph.hxx"

namespace CLAM
{
namespace VM
{
	class QDESIGNER_WIDGET_EXPORT MelSpectrumView : public BarGraph
	{
		Q_OBJECT
		public:
			MelSpectrumView(QWidget * parent, FloatArrayDataSource * dataSource = 0)
				: BarGraph(parent,dataSource)
			{}
			~MelSpectrumView();
	};
}
}


#endif// MelSpectrumView_hxx
