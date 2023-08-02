#include <SDSF/SoundStream.hpp>
#include <iostream>
#include <SDSF/Exception.hpp>

SoundStream* SoundStream::currentStream = nullptr;

mm_word SoundStream::streamFunctionForwarder(mm_word length, mm_addr dest, mm_stream_formats format) {
    return streamFunction(*currentStream, length, dest, format);
}

SoundStream::SoundStream()
{
    stream.manual = false;
    stream.callback = &streamFunctionForwarder;

    /*if(currentStream == nullptr) {
        currentStream = this;
    }else {
        throw Exception("Only one stream is allowed at a time!");
    }*/
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

mm_word SoundStream::streamFunction(SoundStream& stream, mm_word length, mm_addr dest, mm_stream_formats format)
{
    // The amount of bytes to read
    int amountToRead = length;

    if(format == mm_stream_formats::MM_STREAM_8BIT_STEREO || format == mm_stream_formats::MM_STREAM_16BIT_MONO) {
        amountToRead *= 2;
    }else if(format == mm_stream_formats::MM_STREAM_16BIT_STEREO) {
        amountToRead *= 4;
    }

    if(stream.reader.GetBytesLeft() < amountToRead) {
        if(stream.loop) {
            stream.reader.Seek(0);
        }else {
            return 0;
        }
    }

    s16* target = reinterpret_cast<s16*>(dest);

    stream.reader.Read<s16>(target, amountToRead / 2);

    return length;
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