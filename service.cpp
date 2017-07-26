/**
 * Master thesis
 * by Alf-Andre Walla 2016-2017
 *
**/
#include <service>
#include <net/inet4>
#include <cstdio>
#include "../LiveUpdate/liveupdate.hpp"
static void* LIVEUPD_LOCATION = (void*) 0x8000000; // at 128mb

using storage_func_t = liu::LiveUpdate::storage_func;
void setup_liveupdate_server(net::Inet<net::IP4>&, storage_func_t);

void Service::start()
{
  storage_func_t func = nullptr;

  if (liu::LiveUpdate::is_resumable(LIVEUPD_LOCATION) == false)
  {
    auto& inet = net::Inet4::ifconfig<0>(
          { 10,0,0,42 },     // IP
          { 255,255,255,0 }, // Netmask
          { 10,0,0,1 },      // Gateway
          { 10,0,0,1 });     // DNS

    setup_liveupdate_server(inet, func);
  }
  else
  {
    printf("This program has new functionality\n");
  }
}

#include "../LiveUpdate/server.hpp"
void setup_liveupdate_server(net::Inet<net::IP4>& inet, storage_func_t func)
{
  static liu::LiveUpdate::storage_func save_function;
  save_function = func;

  // listen for live updates
  server(inet, 666,
  [] (liu::buffer_t& buffer)
  {
    printf("* Live updating from %p (len=%u)\n",
            buffer.data(), (uint32_t) buffer.size());
    try
    {
      // run live update process
      liu::LiveUpdate::begin(LIVEUPD_LOCATION, buffer, save_function);
    }
    catch (std::exception& err)
    {
      liu::LiveUpdate::restore_environment();
      printf("Live Update location: %p\n", LIVEUPD_LOCATION);
      printf("Live update failed:\n%s\n", err.what());
    }
  });
  printf("LiveUpdate server listening on port 666\n");
}
