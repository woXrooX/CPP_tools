#include <iostream>
#include <fstream>
#include <string>

// #include <thread> // For Threads
// #include <future> // For Async & Await
// #include <chrono> // For std::chrono
// #include <filesystem> // For std::filesystem
// #include <sys/types.h>
// #include <sys/stat.h> // 4 umask()
// #include <unistd.h> // 4 fork() & PID & getpid() & sleep()
// #include <syslog.h> // 4 syslog(), openlog(), closelog(), LOG_PID, LOG_DAEMON, LOG_NOTICE
// #include <csignal> // 4 std::signal()


// #include "tools/blockchain/chain.h"
// #include "tools/client/client.h"
// #include "tools/daemon/daemon.h"
// #include "tools/json/json.h"
// #include "tools/logger/logger.h"
// #include "tools/MET/met.h"
// #include "tools/server/server.h"
// #include "tools/sha256/sha256.h"
// #include "tools/workflow/workflow.h"


////////////////// Lab START
// void lab(){}
////////////////// Lab END

// int main(int argc, char const *argv[]){
int main() {
//////////// Lab Call
  // lab();

//////////// Met
  // woXrooX::MET met;
  // met.lap("500ms");
  // met.showLaps();


//////////// Logger
  // woXrooX::Logger::enableLogToFile();
  // woXrooX::Logger::disableLogToFile();
  //
  // woXrooX::Logger::enableSquareBrackets();
  // woXrooX::Logger::disableSquareBrackets();
  //
  // woXrooX::Log::custom("Custom", "Custom Message");
  // woXrooX::Log::success("SUCCESS");
  // woXrooX::Log::info("INFO");
  // woXrooX::Log::warning("WARNING");
  // woXrooX::Log::error("ERROR");
  // woXrooX::Log::line();
  // woXrooX::Log::newLine();



//////////// Daemon
  // woXrooX::Daemon::start();


//////////// Server
  // woXrooX::Server::start();
  // woXrooX::Server::stop();

//////////// Json
  // woXrooX::Json::parse("Hey");


//////////// BlockChain
  // woXrooX::Block blockA("A Block", "0000000000000000000000000000000000000000000000000000000000000000");
  // woXrooX::Block blockB("B Block", blockA.getHash());
  // woXrooX::Block blockC("C Block", blockB.getHash());
  // woXrooX::Block blockD("D Block", blockC.getHash());
  //
  // woXrooX::Chain chain;
  // chain.addBlockToChain(blockA);
  // chain.addBlockToChain(blockB);
  // chain.addBlockToChain(blockC);
  // chain.addBlockToChain(blockD);
  //
  // chain.getInfos();
  // chain.getBlocksInfos();


//////////// Sha256
  // std::string data = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbcccccccccccccccccccccccccccccccccccccccccccccccccccccczzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
  // std::string data = "Data";
  // woXrooX::Sha256 hash;
  // std::cout << hash.digest(data) << '\n';


//////////// workflow
  // woXrooX::Workflow workflow(argc, argv);


//////////// FileSystem
  // std::cout << std::filesystem::current_path() << '\n';  // Prints Current Path By Invokation Point
  // std::filesystem::create_directories("a/b/c/d");  // Creates All Folders In The Way If Doensn't Exists
  // std::filesystem::create_directory("a/b/c/d");    // Creates Folder d If a, b, c Are Exists

  // std::filesystem::path p = "main.cpp"; // Creates "main.cpp"
  // std::cout << std::filesystem::current_path() << '\n';  // Prints /home/to/current/invokation/folder
  // std::cout << std::filesystem::absolute(p) << '\n';  // Prints /home/to/current/invokation/folder/main.cpp

//////////// Async & Await
  // std::cout << "Main ID: " << std::this_thread::get_id() << '\n';
  // std::cout << "Hardware Concurrency: " << std::thread::hardware_concurrency() << '\n';

  // std::future<void> f = std::async(func_name); // With or without New Thread. below code is the same
  // std::future<void> f = std::async(std::launch::async | std::launch::deferred, func_name); // w/ or w/o New Thread
  // std::future<void> f = std::async(std::launch::async, func_name); // w/ New Thread
  // std::future<void> f = std::async(std::launch::deferred, func_name); // w/o New Thread
  // f.get(); // Calls the func
  // auto a = f.get();

  // std::future<void> fa = std::async(std::launch::deferred, a);
  // std::future<void> fb = std::async(std::launch::deferred, b);
  //
  // fa.wait();
  // fb.wait();


  return 0;
}


// https://www.youtube.com/watch?v=SZQ6-pf-5Us&t=269s thread & async
// std::chrono::milliseconds timespan(2000);
// std::this_thread::sleep_for(timespan);
// std::this_thread::get_id();
