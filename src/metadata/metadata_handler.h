/*MT*
    
    MediaTomb - http://www.mediatomb.cc/
    
    metadata_handler.h - this file is part of MediaTomb.
    
    Copyright (C) 2005 Gena Batyan <bgeradz@mediatomb.cc>,
                       Sergey 'Jin' Bostandzhyan <jin@mediatomb.cc>
    
    Copyright (C) 2006-2010 Gena Batyan <bgeradz@mediatomb.cc>,
                            Sergey 'Jin' Bostandzhyan <jin@mediatomb.cc>,
                            Leonhard Wimmer <leo@mediatomb.cc>
    
    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.
    
    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    version 2 along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
    
    $Id$
*/

/// \file metadata_handler.h
/// \brief Definition of the MetadataHandler class.
#ifndef __METADATA_HANDLER_H__
#define __METADATA_HANDLER_H__

#include "common.h"

#include <array>

// forward declaration
class CdsItem;
class Config;
class IOHandler;

// content handler Id's
#define CH_DEFAULT 0
#define CH_LIBEXIF 1
#define CH_ID3 2
#define CH_TRANSCODE 3
#define CH_EXTURL 4
#define CH_MP4 5
#define CH_FFTH 6
#define CH_FLAC 7
#define CH_FANART 8
#define CH_MATROSKA 9
#define CH_SUBTITLE 10
#define CH_RESOURCE 20

#define CONTENT_TYPE_MP3 "mp3"
#define CONTENT_TYPE_OGG "ogg"
#define CONTENT_TYPE_FLAC "flac"
#define CONTENT_TYPE_WMA "wma"
#define CONTENT_TYPE_WAVPACK "wv"
#define CONTENT_TYPE_APE "ape"
#define CONTENT_TYPE_JPG "jpg"
#define CONTENT_TYPE_PLAYLIST "playlist"
#define CONTENT_TYPE_MP4 "mp4"
#define CONTENT_TYPE_PCM "pcm"
#define CONTENT_TYPE_AVI "avi"
#define CONTENT_TYPE_MPEG "mpeg"
#define CONTENT_TYPE_QUICKTIME "quicktime"
#define CONTENT_TYPE_MKV "mkv"
#define CONTENT_TYPE_MKA "mka"
#define CONTENT_TYPE_AIFF "aiff"
#define CONTENT_TYPE_DSD "dsd"

#define OGG_THEORA "t"

#define RESOURCE_CONTENT_TYPE "rct"
#define RESOURCE_HANDLER "rh"

#define ID3_ALBUM_ART "aa"
#define VIDEO_SUB "vs"

#define EXIF_THUMBNAIL "EX_TH"
#define THUMBNAIL "th" // thumbnail without need for special handling

typedef enum {
    M_TITLE = 0,
    M_ARTIST,
    M_ALBUM,
    M_DATE,
    M_UPNP_DATE,
    M_GENRE,
    M_DESCRIPTION,
    M_LONGDESCRIPTION,
    M_TRACKNUMBER,
    M_ALBUMARTURI,
    M_REGION,
    /// \todo make sure that those are only used with appropriate upnp classes
    M_AUTHOR,
    M_DIRECTOR,
    M_PUBLISHER,
    M_RATING,
    M_ACTOR,
    M_PRODUCER,
    M_ALBUMARTIST,

    // Classical Music Related Fields
    M_COMPOSER,
    M_CONDUCTOR,
    M_ORCHESTRA,

    M_MAX
} metadata_fields_t;

constexpr std::array<std::pair<metadata_fields_t, const char*>, M_MAX> mt_keys = { {
    { M_TITLE, "dc:title" },
    { M_ARTIST, "upnp:artist" },
    { M_ALBUM, "upnp:album" },
    { M_DATE, "dc:date" },
    { M_UPNP_DATE, "upnp:date" },
    { M_GENRE, "upnp:genre" },
    { M_DESCRIPTION, "dc:description" },
    { M_LONGDESCRIPTION, "upnp:longDescription" },
    { M_TRACKNUMBER, "upnp:originalTrackNumber" },
    { M_ALBUMARTURI, "upnp:albumArtURI" },
    { M_REGION, "upnp:region" },
    { M_AUTHOR, "upnp:author" },
    { M_DIRECTOR, "upnp:director" },
    { M_PUBLISHER, "dc:publisher" },
    { M_RATING, "upnp:rating" },
    { M_ACTOR, "upnp:actor" },
    { M_PRODUCER, "upnp:producer" },
    { M_ALBUMARTIST, "upnp:artist@role[AlbumArtist]" },
    { M_COMPOSER, "upnp:composer" },
    { M_CONDUCTOR, "upnp:conductor" },
    { M_ORCHESTRA, "upnp:orchestra" },
} };

// res tag attributes
typedef enum {
    R_SIZE = 0,
    R_DURATION,
    R_BITRATE,
    R_SAMPLEFREQUENCY,
    R_NRAUDIOCHANNELS,
    R_RESOLUTION,
    R_COLORDEPTH,
    R_PROTOCOLINFO,
    R_RESOURCE_FILE,
    R_TYPE,
    R_MAX
} resource_attributes_t;

constexpr std::array<std::pair<resource_attributes_t, const char*>, R_MAX> res_keys = { {
    { R_SIZE, "size" },
    { R_DURATION, "duration" },
    { R_BITRATE, "bitrate" },
    { R_SAMPLEFREQUENCY, "sampleFrequency" },
    { R_NRAUDIOCHANNELS, "nrAudioChannels" },
    { R_RESOLUTION, "resolution" },
    { R_COLORDEPTH, "colorDepth" },
    { R_PROTOCOLINFO, "protocolInfo" },
    { R_RESOURCE_FILE, "resFile" },
    { R_TYPE, "type" },
} };

/// \brief This class is responsible for providing access to metadata information
/// of various media.
class MetadataHandler {
protected:
    std::shared_ptr<Config> config;

public:
    /// \brief Definition of the supported metadata fields.

    explicit MetadataHandler(std::shared_ptr<Config> config);

    static void setMetadata(const std::shared_ptr<Config>& config, const std::shared_ptr<CdsItem>& item);
    static std::string getMetaFieldName(metadata_fields_t field);
    static std::string getResAttrName(resource_attributes_t attr);
    static std::unique_ptr<MetadataHandler> createHandler(const std::shared_ptr<Config>& config, int handlerType);

    virtual void fillMetadata(std::shared_ptr<CdsItem> item) = 0;
    virtual std::unique_ptr<IOHandler> serveContent(std::shared_ptr<CdsItem> item, int resNum) = 0;
    virtual std::string getMimeType();

    static const char* mapContentHandler2String(int ch);

    virtual ~MetadataHandler() = default;
};

#endif // __METADATA_HANDLER_H__
