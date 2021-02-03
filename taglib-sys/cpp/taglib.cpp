#include <taglib/mpegfile.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/id3v2tag.h>
#include <taglib/tag_c.h>

using namespace TagLib;

typedef struct { int dummy; } TagLib_TagId3v2;

extern "C" TagLib_TagId3v2 *taglib_file_tag_id3v2(TagLib_File *file, bool create)
{
  auto f = dynamic_cast<TagLib::MPEG::File*>(reinterpret_cast<TagLib::File *>(file));
  return f ? reinterpret_cast<TagLib_TagId3v2 *>(f->ID3v2Tag(create)) : nullptr;
}

extern "C" void test(const char *filename, const uint8_t* buffer, size_t buffer_size) {
  TagLib::MPEG::File file(filename);
  auto id3v2Tag = file.ID3v2Tag(true);
  if (id3v2Tag) {
    TagLib::ID3v2::FrameList frames = id3v2Tag->frameList("APIC");
    TagLib::ID3v2::AttachedPictureFrame *frame = 0;

    if (frames.isEmpty())
    {
      frame = new TagLib::ID3v2::AttachedPictureFrame();
      id3v2Tag->addFrame(frame);
    }
    else
    {
      frame = static_cast<TagLib::ID3v2::AttachedPictureFrame *>(frames.front());
    }

    frame->setType(TagLib::ID3v2::AttachedPictureFrame::FrontCover);

    TagLib::ByteVector bv((const char*)buffer, buffer_size);
    frame->setPicture(bv);
  }
}