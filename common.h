/*!
 \file common.h

*/

#ifndef COMMON_H
#define COMMON_H


#include <QDebug>


#define THUMBNAIL_WIDTH		160
#define THUMBNAIL_HEIGHT	120


#ifdef __GNUC__
	#define myDebug qDebug() << __FILE__ << "(" << __LINE__ << ") - " << __PRETTY_FUNCTION__ << ":"
#elif __MINGW32__
	#define myDebug qDebug() << __FILE__ << "(" << __LINE__ << ") - " << __PRETTY_FUNCTION__ << ":"
#else
	#define myDebug qDebug() << __FILE__ << "(" << __LINE__ << ") - " << __FUNCTION__ << ":"
#endif

#define ITEM_MANUFACTURER_DATA		(Qt::UserRole+1)
#define ITEM_DISTRIBUTOR_DATA		(Qt::UserRole+2)
#define ITEM_STORAGE_CATEGORY_DATA	(Qt::UserRole+3)
#define ITEM_STORAGE_DATA			(Qt::UserRole+4)
#define ITEM_PART_DATA				(Qt::UserRole+5)
#define ITEM_PROJECT_DATA			(Qt::UserRole+6)

#define TAB_MANUFACTURER_LIST		1
#define TAB_DISTRIBUTOR_LIST		2
#define TAB_STORAGE_LIST			3
#define TAB_PART_LIST				4
#define TAB_PROJECT_LIST			5


#endif // COMMON_H
