#include <taglib/attachedpictureframe.h>
#include <taglib/id3v2tag.h>
#include <taglib/mpegfile.h>
#include <taglib/tag_c.h>

using namespace TagLib;

typedef struct {
    int dummy;
} TagLib_TagId3v2;

extern "C" TagLib_TagId3v2 *taglib_file_tag_id3v2(TagLib_File *file,
                                                  bool create) {
    auto f = dynamic_cast<MPEG::File *>(
            reinterpret_cast<File *>(file));
    return f ? reinterpret_cast<TagLib_TagId3v2 *>(f->ID3v2Tag(create)) : nullptr;
}

extern "C" bool taglib_id3v2_add_picture(TagLib_TagId3v2 *tag, uint32_t type,
                                         const uint8_t *buffer,
                                         size_t buffer_size) {
    auto id3v2Tag = reinterpret_cast<ID3v2::Tag *>(tag);
    if (!id3v2Tag) {
        return false;
    }
    ID3v2::FrameList frames = id3v2Tag->frameList("APIC");
    ID3v2::AttachedPictureFrame *frame = 0;

    frame = new ID3v2::AttachedPictureFrame();
    id3v2Tag->addFrame(frame);
    frame->setType(static_cast<ID3v2::AttachedPictureFrame::Type>(type));
    ByteVector bv((const char *) buffer, buffer_size);
    frame->setPicture(bv);
    return true;
}