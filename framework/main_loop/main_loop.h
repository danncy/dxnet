#ifndef FRAMEWORK_MAIN_LOOP_H_
#define FRAMEWORK_MAIN_LOOP_H_

namespace framework {

/*
 * MainLoop use to execute Task in the current thread(as pthread.) 
 * the Task types will be CONTROL and NORMAL.
 * CONTROL task make the mainloop to start,pause,stop.
 * NORMARL task is running in the mainloop, 
 * with their current process and thread access rights.
 */
struct MainLoop {
  MainLoop();
  ~MainLoop();

  static MainLoop* current();
  void Run();

  bool Start();
  bool Pause();
  bool Stop();

private:
  
};

} // namespace framework
#endif
