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

#include "image.h"

Image::Image()
{
}

Image::Image(int width, int height, QImage::Format format)
    : image(width,height,format)
{
}

Image::Image(const QImage &image)
{
    this->image=image;
}

Image::Image(const char *file_name)
    : image(file_name)
{
}

Image::Image(const std::string &file_name)
    : image(file_name.c_str())
{
}

bool Image::load(const char *file_name)
{
#ifdef QT_VERSION
    return image.load(file_name);
#else
    #error Please implement image importing!
#endif
}

int Image::width() const
{
    return image.width();
}

int Image::height() const
{
    return image.height();
}

unsigned char * Image::bits()
{
    return image.bits();
}

void Image::mirror(bool horizontal, bool vertical)
{
    this->image=image.mirrored(horizontal,vertical);
}

bool Image::save(const char *path, const char * format)
{
    return image.save(path, format);
}

bool Image::save(const std::string &path,const char * format)
{
    return save(path.c_str(), format);
}

Image Image::convertToGLFormat() const
{
    QImage res(image.size(), QImage::Format_ARGB32);

    convertToGLFormatHelper(res, image.convertToFormat(QImage::Format_ARGB32), GL_RGBA);
    return Image(res);
}

std::string Image::supportedFormats()
{
    std::string supportedImageFormats;
    for (int formatIndex = 0; formatIndex < QImageReader::supportedImageFormats().count(); formatIndex++) {
        supportedImageFormats += " *.";
        supportedImageFormats += QImageReader::supportedImageFormats()[formatIndex].constData();
    }
    return supportedImageFormats;
}

static inline QRgb qt_gl_convertToGLFormatHelper(QRgb src_pixel, GLenum texture_format)
{
    if (texture_format == GL_BGRA) {

        if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {

            return ((src_pixel << 24) & 0xff000000)
                   | ((src_pixel >> 24) & 0x000000ff)
                   | ((src_pixel << 8) & 0x00ff0000)
                   | ((src_pixel >> 8) & 0x0000ff00);

        } else {

            return src_pixel;

        }

    } else {  // GL_RGBA

        if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {

            return (src_pixel << 8) | ((src_pixel >> 24) & 0xff);

        } else {

            return ((src_pixel << 16) & 0xff0000)
                   | ((src_pixel >> 16) & 0xff)
                   | (src_pixel & 0xff00ff00);

        }

    }
}
/*

QRgb qt_gl_convertToGLFormat(QRgb src_pixel, GLenum texture_format)
{
    return qt_gl_convertToGLFormatHelper(src_pixel, texture_format);
}*/

void Image::convertToGLFormatHelper(QImage &dst, const QImage &img, GLenum texture_format) const
{
    Q_ASSERT(dst.depth() == 32);

    Q_ASSERT(img.depth() == 32);

    if (dst.size() != img.size()) {

        int target_width = dst.width();

        int target_height = dst.height();

        float sx = target_width / float(img.width());

        float sy = target_height / float(img.height());

        quint32 *dest = (quint32 *) dst.scanLine(0); // NB! avoid detach here

        uchar *srcPixels = (uchar *) img.scanLine(img.height() - 1);

        int sbpl = img.bytesPerLine();

        int dbpl = dst.bytesPerLine();

        int ix = int(0x00010000 / sx);

        int iy = int(0x00010000 / sy);

        quint32 basex = int(0.5 * ix);

        quint32 srcy = int(0.5 * iy);

        // scale, swizzle and mirror in one loop

        while (target_height--) {

            const uint *src = (const quint32 *) (srcPixels - (srcy >> 16) * sbpl);

            int srcx = basex;

            for (int x=0; x<target_width; ++x) {

                dest[x] = qt_gl_convertToGLFormatHelper(src[srcx >> 16], texture_format);

                srcx += ix;

            }

            dest = (quint32 *)(((uchar *) dest) + dbpl);

            srcy += iy;

        }

    } else {

        const int width = img.width();
        const int height = img.height();
        const uint *p = (const uint*) img.scanLine(img.height() - 1);
        uint *q = (uint*) dst.scanLine(0);

        if (texture_format == GL_BGRA) {

            if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {

                // mirror + swizzle

                for (int i=0; i < height; ++i) {

                    const uint *end = p + width;

                    while (p < end) {

                        *q = ((*p << 24) & 0xff000000)
                             | ((*p >> 24) & 0x000000ff)
                             | ((*p << 8) & 0x00ff0000)
                             | ((*p >> 8) & 0x0000ff00);
                        p++;
                        q++;
                    }

                    p -= 2 * width;

                }

            } else {

                const uint bytesPerLine = img.bytesPerLine();

                for (int i=0; i < height; ++i) {

                    memcpy(q, p, bytesPerLine);
                    q += width;
                    p -= width;
                }
            }

        } else {

            if (QSysInfo::ByteOrder == QSysInfo::BigEndian) {

                for (int i=0; i < height; ++i) {

                    const uint *end = p + width;

                    while (p < end) {

                        *q = (*p << 8) | ((*p >> 24) & 0xff);
                        p++;
                        q++;
                    }
                    p -= 2 * width;
                }

            } else {

                for (int i=0; i < height; ++i) {

                    const uint *end = p + width;

                    while (p < end) {

                        *q = ((*p << 16) & 0xff0000) | ((*p >> 16) & 0xff) | (*p & 0xff00ff00);
                        p++;
                        q++;
                    }

                    p -= 2 * width;
                }
            }
        }
    }
}
