/*
   Copyright (C) 2007 Maximilian Kossick <maximilian.kossick@googlemail.com>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
*/

#include "MetaUtility.h"

#include "Meta.h"
#include "meta/Capability.h"
#include "meta/EditCapability.h"

#include <QChar>

        static const QString XESAM_ALBUM          = "http://freedesktop.org/standards/xesam/1.0/core#album";
        static const QString XESAM_ARTIST         = "http://freedesktop.org/standards/xesam/1.0/core#artist";
        static const QString XESAM_BITRATE        = "http://freedesktop.org/standards/xesam/1.0/core#audioBitrate";
        static const QString XESAM_BPM            = "http://freedesktop.org/standards/xesam/1.0/core#audioBPM";
        static const QString XESAM_CODEC          = "http://freedesktop.org/standards/xesam/1.0/core#audioCodec";
        static const QString XESAM_COMMENT        = "http://freedesktop.org/standards/xesam/1.0/core#comment";
        static const QString XESAM_COMPOSER       = "http://freedesktop.org/standards/xesam/1.0/core#composer";
        static const QString XESAM_DISCNUMBER     = "http://freedesktop.org/standards/xesam/1.0/core#discNumber";
        static const QString XESAM_FILESIZE       = "http://freedesktop.org/standards/xesam/1.0/core#size";
        static const QString XESAM_GENRE          = "http://freedesktop.org/standards/xesam/1.0/core#genre";
        static const QString XESAM_LENGTH         = "http://freedesktop.org/standards/xesam/1.0/core#mediaDuration";
        static const QString XESAM_RATING         = "http://freedesktop.org/standards/xesam/1.0/core#userRating";
        static const QString XESAM_SAMPLERATE     = "http://freedesktop.org/standards/xesam/1.0/core#audioSampleRate";
        static const QString XESAM_TITLE          = "http://freedesktop.org/standards/xesam/1.0/core#title";
        static const QString XESAM_TRACKNUMBER    = "http://freedesktop.org/standards/xesam/1.0/core#trackNumber";
        static const QString XESAM_URL            = "http://freedesktop.org/standards/xesam/1.0/core#url";
        static const QString XESAM_YEAR           = "http://freedesktop.org/standards/xesam/1.0/core#contentCreated";

        static const QString XESAM_SCORE          = "http://freedesktop.org/standards/xesam/1.0/core#autoRating";
        static const QString XESAM_PLAYCOUNT      = "http://freedesktop.org/standards/xesam/1.0/core#useCount";
        static const QString XESAM_FIRST_PLAYED   = "http://freedesktop.org/standards/xesam/1.0/core#firstUsed";
        static const QString XESAM_LAST_PLAYED    = "http://freedesktop.org/standards/xesam/1.0/core#lastUsed";

        //static bool conversionMapsInitialised = false;

QVariantMap
Meta::Field::mapFromTrack( const Meta::Track *track )
{
    //note: track does not support bpm, first_played yet
    QVariantMap map;
    if( !track )
        return map;

    if( track->name().isEmpty() )
        map.insert( Meta::Field::TITLE, QVariant( track->prettyName() ) );
    else
        map.insert( Meta::Field::TITLE, QVariant( track->name() ) );
    if( !track->artist()->name().isEmpty() )
        map.insert( Meta::Field::ARTIST, QVariant( track->artist()->name() ) );
    if( !track->album()->name().isEmpty() )
        map.insert( Meta::Field::ALBUM, QVariant( track->album()->name() ) );
    if( track->filesize() )
        map.insert( Meta::Field::FILESIZE, QVariant( track->filesize() ) );
    if( !track->genre()->name().isEmpty() )
        map.insert( Meta::Field::GENRE, QVariant( track->genre()->name() ) );
    if( !track->composer()->name().isEmpty() )
        map.insert( Meta::Field::COMPOSER, QVariant( track->composer()->name() ) );
    if( !track->year()->name().isEmpty() )
        map.insert( Meta::Field::YEAR, QVariant( track->year()->name() ) );
    if( !track->comment().isEmpty() )
        map.insert( Meta::Field::COMMENT, QVariant( track->comment() ) );
    if( track->trackNumber() )
        map.insert( Meta::Field::TRACKNUMBER, QVariant( track->trackNumber() ) );
    if( track->discNumber() )
        map.insert( Meta::Field::DISCNUMBER, QVariant( track->discNumber() ) );
    if( track->bitrate() )
        map.insert( Meta::Field::BITRATE, QVariant( track->bitrate() ) );
    if( track->length() )
        map.insert( Meta::Field::LENGTH, QVariant( track->length() ) );
    if( track->sampleRate() )
        map.insert( Meta::Field::SAMPLERATE, QVariant( track->sampleRate() ) );

    map.insert( Meta::Field::URL, QVariant( track->prettyUrl() ) );
    map.insert( Meta::Field::RATING, QVariant( track->rating() ) );
    map.insert( Meta::Field::SCORE, QVariant( track->score() ) );
    map.insert( Meta::Field::PLAYCOUNT, QVariant( track->playCount() ) );
    map.insert( Meta::Field::LAST_PLAYED, QVariant( track->lastPlayed() ) );
    
    return map;
}


void
Meta::Field::updateTrack( Meta::Track *track, const QVariantMap &metadata )
{
    if( !track || !track->hasCapabilityInterface( Meta::Capability::Editable ) )
        return;

    Meta::EditCapability *ec = track->as<Meta::EditCapability>();
    if( !ec || !ec->isEditable() )
        return;
    ec->beginMetaDataUpdate();
    QString title = metadata.contains( Meta::Field::TITLE ) ?
                            metadata.value( Meta::Field::TITLE ).toString() : QString();
    ec->setTitle( title );
    QString comment = metadata.contains( Meta::Field::COMMENT ) ?
                            metadata.value( Meta::Field::COMMENT ).toString() : QString();
    ec->setComment( comment );
    int tracknr = metadata.contains( Meta::Field::TRACKNUMBER ) ?
                            metadata.value( Meta::Field::TRACKNUMBER ).toInt() : 0;
    ec->setTrackNumber( tracknr );
    int discnr = metadata.contains( Meta::Field::DISCNUMBER ) ?
                            metadata.value( Meta::Field::DISCNUMBER ).toInt() : 0;
    ec->setDiscNumber( discnr );
    QString artist = metadata.contains( Meta::Field::ARTIST ) ?
                            metadata.value( Meta::Field::ARTIST ).toString() : QString();
    ec->setArtist( artist );
    QString album = metadata.contains( Meta::Field::ALBUM ) ?
                            metadata.value( Meta::Field::ALBUM ).toString() : QString();
    ec->setAlbum( album );
    QString genre = metadata.contains( Meta::Field::GENRE ) ?
                            metadata.value( Meta::Field::GENRE ).toString() : QString();
    ec->setGenre( genre );
    QString composer = metadata.contains( Meta::Field::COMPOSER ) ?
                            metadata.value( Meta::Field::COMPOSER ).toString() : QString();
    ec->setComposer( composer );
    QString year = metadata.contains( Meta::Field::YEAR ) ?
                            metadata.value( Meta::Field::YEAR ).toString() : QString();
    ec->setYear( year );

    ec->endMetaDataUpdate();
}

QString
Meta::Field::xesamPrettyToFullFieldName( const QString &name )
{
    if( name == Meta::Field::ARTIST )
        return XESAM_ARTIST;
    else if( name == Meta::Field::ALBUM )
        return XESAM_ALBUM;
    else if( name == Meta::Field::BITRATE )
        return XESAM_BITRATE;
    else if( name == Meta::Field::BPM )
        return XESAM_BPM;
    else if( name == Meta::Field::CODEC )
        return XESAM_CODEC;
    else if( name == Meta::Field::COMMENT )
        return XESAM_COMMENT;
    else if( name == Meta::Field::COMPOSER )
        return XESAM_COMPOSER;
    else if( name == Meta::Field::DISCNUMBER )
        return XESAM_DISCNUMBER;
    else if( name == Meta::Field::FILESIZE )
        return XESAM_FILESIZE;
    else if( name == Meta::Field::GENRE )
        return XESAM_GENRE;
    else if( name == Meta::Field::LENGTH )
        return XESAM_LENGTH;
    else if( name == Meta::Field::RATING )
        return XESAM_RATING;
    else if( name == Meta::Field::SAMPLERATE )
        return XESAM_SAMPLERATE;
    else if( name == Meta::Field::TITLE )
        return XESAM_TITLE;
    else if( name == Meta::Field::TRACKNUMBER )
        return XESAM_TRACKNUMBER;
    else if( name == Meta::Field::URL )
        return XESAM_URL;
    else if( name == Meta::Field::YEAR )
        return XESAM_YEAR;
    else if( name==Meta::Field::SCORE )
        return XESAM_SCORE;
    else if( name==Meta::Field::PLAYCOUNT )
        return XESAM_PLAYCOUNT;
    else if( name==Meta::Field::FIRST_PLAYED )
        return XESAM_FIRST_PLAYED;
    else if( name==Meta::Field::LAST_PLAYED )
        return XESAM_LAST_PLAYED;
    else
        return "xesamPrettyToFullName: unknown name " + name;
}

QString
Meta::Field::xesamFullToPrettyFieldName( const QString &name )
{
    if( name == XESAM_ARTIST )
        return Meta::Field::ARTIST;
    else if( name == XESAM_ALBUM )
        return Meta::Field::ALBUM;
    else if( name == XESAM_BITRATE )
        return Meta::Field::BITRATE;
    else if( name == XESAM_BPM )
        return Meta::Field::BPM;
    else if( name == XESAM_CODEC )
        return Meta::Field::CODEC;
    else if( name == XESAM_COMMENT )
        return Meta::Field::COMMENT;
    else if( name == XESAM_COMPOSER )
        return Meta::Field::COMPOSER;
    else if( name == XESAM_DISCNUMBER )
        return Meta::Field::DISCNUMBER;
    else if( name == XESAM_FILESIZE )
        return Meta::Field::FILESIZE;
    else if( name == XESAM_GENRE )
        return Meta::Field::GENRE;
    else if( name == XESAM_LENGTH )
        return Meta::Field::LENGTH;
    else if( name == XESAM_RATING )
        return Meta::Field::RATING;
    else if( name == XESAM_SAMPLERATE )
        return Meta::Field::SAMPLERATE;
    else if( name == XESAM_TITLE )
        return Meta::Field::TITLE;
    else if( name == XESAM_TRACKNUMBER )
        return Meta::Field::TRACKNUMBER;
    else if( name == XESAM_URL )
        return Meta::Field::URL;
    else if( name == XESAM_YEAR )
        return Meta::Field::YEAR;
    else if( name == XESAM_SCORE )
        return Meta::Field::SCORE;
    else if( name == XESAM_PLAYCOUNT )
        return Meta::Field::PLAYCOUNT;
    else if( name == XESAM_FIRST_PLAYED )
        return Meta::Field::FIRST_PLAYED;
    else if( name == XESAM_LAST_PLAYED )
        return Meta::Field::LAST_PLAYED;
    else
        return "xesamFullToPrettyName: unknown name " + name;
}


QString
Meta::msToPrettyTime( int ms )
{
    return Meta::secToPrettyTime( ms / 1000 );
}

QString
Meta::secToPrettyTime( int seconds )
{
    int minutes = ( seconds / 60 ) % 60;
    int hours = seconds / 3600;
    QString s = QChar( ':' );
    s.append( ( seconds % 60 ) < 10 ? QString( "0%1" ).arg( seconds % 60 ) : QString::number( seconds % 60 ) ); //seconds

    if( hours )
    {
        s.prepend( minutes < 10 ? QString( "0%1" ).arg( minutes ) : QString::number( minutes ) );
        s.prepend( ':' );
    }
    else
    {
        s.prepend( QString::number( minutes ) );
        return s;
    }

    //don't zeroPad the last one, as it can be greater than 2 digits
    s.prepend( QString::number( hours ) );

    return s;
}
