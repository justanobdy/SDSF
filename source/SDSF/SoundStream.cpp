#include <SDSF/SoundStream.hpp>
#include <SDSF/Exception.hpp>
#include <cmath>

SoundStream* SoundStream::currentStream = nullptr;

mm_word SoundStream::streamFunctionForwarder(mm_word length, mm_addr dest, mm_stream_formats format) {
    return streamFunction(*currentStream, length, dest, format);
}

SoundStream::SoundStream()
{
    stream.manual = false;
    stream.callback = &streamFunctionForwarder;
}

void SoundStream::Play(const std::string& filename) {
    if(currentStream != nullptr) {
        mmStreamClose();
    }

    reader.OpenFile(filename);

    currentStream = this;
    mmStreamOpen(&stream);
}

void SoundStream::Pause()
{
    mmStreamClose();
}

void SoundStream::Resume()
{
    if(!reader.IsOpen())
        throw Exception("Cannot resume stream: No file open!");
    
    currentStream = this;

    mmStreamOpen(&stream);
}

u8 SoundStream::GetBytesPerSample(mm_stream_formats format) {
    switch(format) {
        case mm_stream_formats::MM_STREAM_8BIT_MONO:
            return 1;
        case mm_stream_formats::MM_STREAM_8BIT_STEREO:
        case mm_stream_formats::MM_STREAM_16BIT_MONO:
            return 2;
        case mm_stream_formats::MM_STREAM_16BIT_STEREO:
            return 4;
    }
    
    return 0;
}

mm_word SoundStream::streamFunction(SoundStream& stream, mm_word length, mm_addr dest, mm_stream_formats format)
{
    // The amount of bytes to read
    int amountToRead = length * GetBytesPerSample(format);

    if(stream.reader.GetBytesLeft() < amountToRead) {
        amountToRead = stream.reader.GetBytesLeft();
    }

    if(stream.reader.GetBytesLeft() < GetBytesPerSample(format)) {
        if(stream.loop) {
            stream.reader.Seek(0);
        }else {
            stream.Stop();
            return 0;
        }
    }

    s16* target = reinterpret_cast<s16*>(dest);

    if(amountToRead % 4 != 0) {
        amountToRead = (amountToRead >> 2) << 2;
    }

    stream.reader.Read<s16>(target, amountToRead >> 1);

    return amountToRead / GetBytesPerSample(format);
}

void SoundStream::SetLoop(bool loop)
{
    this->loop = loop;
}

void SoundStream::SetSamplingRate(u32 rate) {
    stream.sampling_rate = rate;
}

void SoundStream::SetFormat(mm_stream_formats format) {
    stream.format = format;
}

void SoundStream::SetBufferLength(u32 length) {
    stream.buffer_length = length;
}

void SoundStream::SetTimer(u32 timer) {
    stream.timer = timer;
}

void SoundStream::Stop() {
    mmStreamClose();
    currentStream = nullptr;
    reader.Close();
}

SoundStream::~SoundStream() {
    Stop();
}