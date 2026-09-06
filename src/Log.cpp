#include "endergfx/Log.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace endergfx {

namespace {

LogCallback g_logCallback = nullptr;

const char *levelToString(LogLevel level) {
  switch (level) {
  case LogLevel::Debug:
    return "DEBUG";
  case LogLevel::Info:
    return "INFO";
  case LogLevel::Warning:
    return "WARN";
  case LogLevel::Error:
    return "ERROR";
  }
  return "UNKNOWN";
}

void defaultLogCallback(LogLevel level, const std::string &message) {
  std::FILE *stream = (level == LogLevel::Error || level == LogLevel::Warning)
                          ? stderr
                          : stdout;
  std::fprintf(stream, "[EnderGFX] [%s] %s\n", levelToString(level),
               message.c_str());
}

LogLevel detectDefaultLogLevel() {
  if (std::getenv("ENDERGFX_DEBUG")) {
    return LogLevel::Debug;
  }
  const char *env = std::getenv("ENDERGFX_LOG_LEVEL");
  if (!env) {
    return LogLevel::Warning;
  }
  if (std::strcmp(env, "debug") == 0)
    return LogLevel::Debug;
  if (std::strcmp(env, "info") == 0)
    return LogLevel::Info;
  if (std::strcmp(env, "warning") == 0)
    return LogLevel::Warning;
  if (std::strcmp(env, "error") == 0)
    return LogLevel::Error;
  return LogLevel::Warning;
}

LogLevel g_minLevel = detectDefaultLogLevel();

} // namespace

void setLogCallback(LogCallback callback) {
  g_logCallback = std::move(callback);
}

void setMinLogLevel(LogLevel level) { g_minLevel = level; }

LogLevel getMinLogLevel() { return g_minLevel; }

void log(LogLevel level, const std::string &message) {
  if (level < g_minLevel) {
    return;
  }
  if (g_logCallback) {
    g_logCallback(level, message);
  } else {
    defaultLogCallback(level, message);
  }
}

} // namespace endergfx
