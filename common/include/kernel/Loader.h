/**
 * @file Loader.h
 */

#ifndef __LOADER_H__
#define __LOADER_H__

#include "types.h"
#include "Thread.h"
#include "Scheduler.h"
#include "Mutex.h"
#include "ArchMemory.h"
#include "ElfFormat.h"
#include <uvector.h>

class Stabs2DebugInfo;

/**
* @class Loader manages the Addressspace creation of a thread
*/
class Loader
{
  public:

    /**
     *Constructor
     * @param fd the file descriptor of the executable
     * @return Loader instance
     */
    Loader(ssize_t fd);

    /**
     *Destructor
     */
    ~Loader();

    /**
     *Initialises the Addressspace of the User, creates the Thread's
     *InfosUserspaceThread and sets the PageDirectory,
     *loads the ehdr and phdrs from executable
     * @return true if this was successful, false otherwise
     */
    bool loadExecutableAndInitProcess();

    /**
     *loads one page slow by its virtual address: gets a free page, maps it,
     *zeros it out, copies the page, one byte at a time
     * @param virtual_address virtual address where to find the page to load
     */
    void loadPage(pointer virtual_address);

    /**
     * Returns debug info for the loaded userspace program, if available
     */
    Stabs2DebugInfo const* getDebugInfos() const;

    /**
     * Returns debug info for the loaded userspace program, if available
     */
    void* getEntryFunction() const;

    ArchMemory arch_memory_;

  private:

    /**
     *reads ELF-headers from the executable
     * @return true if this was successful, false otherwise
     */
    bool readHeaders();


    /**
     * clean up and sort the elf headers for faster access.
     */
    bool cleanAndSortHeaders();


    bool loadDebugInfoIfAvailable();


    bool readFromBinary (char* buffer, l_off_t position, size_t count);


    size_t fd_;
    Elf::Ehdr *hdr_;
    ustl::list<Elf::Phdr> phdrs_;
    Mutex program_binary_lock_;

    Stabs2DebugInfo *userspace_debug_info_;

};

#endif
