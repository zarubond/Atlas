/**
 *  Atlas - Volumetric terrain editor
 *  Copyright (C) 2012-2015  Ondřej Záruba
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include <QImageReader>
#include "../graphics/opengl.h"

class Image
{
public:
    Image();
    Image(int width, int height, QImage::Format format);
    Image(const QImage & image);
    Image(const char * file_name);
    Image(const std::string& file_name);
    bool load(const char * file_name);
    int width() const;
    int height() const;
    unsigned char *bits();
    void mirror(bool horizontal=true, bool vertical=false);
    bool save(const char * path, const char *format=NULL);

    Image convertToGLFormat() const;
    static std::string supportedFormats();
    bool save(const std::string &path, const char *format);
private:
#ifdef QT_VERSION
    void convertToGLFormatHelper(QImage &dst, const QImage &image, GLenum texture_format) const;
    QImage image;
#else
    #error Please implement image importing!
#endif
};

#endif // IMAGE_H
