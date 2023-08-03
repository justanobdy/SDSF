#pragma once

#include <string>
#include <functional>
#include <array>

#include <maxmod9.h>
#include <nds/ndstypes.h>

#include <SDSF/FileReader.hpp>


class SoundStream {
public:
    /// @brief Constructor
    SoundStream();
    /// @brief Destructor
    ~SoundStream();

    static u8 GetBytesPerSample(mm_stream_formats format);

    /// @brief Start the SoundStream
    /// @param filename The filename to play 
    void Play(const std::string& filename);
    /// @brief Pause the SoundStream
    void Pause();
    /// @brief Resume the SoundStream (if paused)
    void Resume();
    /// @brief Stop the SoundStream
    void Stop();

    /// @brief Set if this stream loops
    /// @param loop If this stream loops
    void SetLoop(bool loop);

    /// @brief Set the sampling rate
    /// @param rate The sampling rate
    void SetSamplingRate(u32 rate);
    /// @brief Set the format of the data
    /// @param format The data format
    void SetFormat(mm_stream_formats format);

    /// @brief Set the buffer length
    /// @param length The buffer length
    void SetBufferLength(u32 length);
    /// @brief Set which hardware timer this stream uses
    /// @param timer The hardware timer
    void SetTimer(u32 timer);
private:
    /// @brief Function which streams the file
    /// @param stream The stream that will be used
    /// @param length The amount of samples to read
    /// @param dest The destination
    /// @param format The format
    /// @return The amount of samples actually read
    static mm_word streamFunction(SoundStream& stream, mm_word length, mm_addr dest, mm_stream_formats format);

    /// @brief Forwarder for the stream function
    /// @param length The amount of samples to read
    /// @param dest The destination
    /// @param format the format of the data
    /// @return The amount of samples actually read
    static mm_word streamFunctionForwarder(mm_word length, mm_addr dest, mm_stream_formats format);

    /// @brief The stream that streamFunction() will use
    static SoundStream* currentStream;

    FileReader reader;
    mm_stream stream;

    bool loop = false;
    bool paused = false;
};