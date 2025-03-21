/*
 * See the dyninst/COPYRIGHT file for copyright information.
 * 
 * We provide the Paradyn Tools (below described as "Paradyn")
 * on an AS IS basis, and do not warrant its validity or performance.
 * We reserve the right to update, modify, or discontinue this
 * software at any time.  We shall have no obligation to supply such
 * updates or modifications or any other form of support to you.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/************************************************************************
 * $Id: Object.h,v 1.20 2008/06/23 18:45:48 legendre Exp $
 * Object.h: interface to objects, symbols, lines and instructions.
************************************************************************/


#if !defined(_Object_h_)
#define _Object_h_

/************************************************************************
 * header files.
************************************************************************/

// trace data streams
#include <iosfwd>
#include <utility>
#include <string>
#include <vector>

#include "Symbol.h"
#include "Symtab.h"
#include "Module.h"
#include "LineInformation.h"
#include "common/src/headers.h"
#include "common/src/MappedFile.h"
#include "common/src/lprintf.h"

namespace Dyninst{
namespace SymtabAPI{

extern bool symbol_compare(const Symbol *s1, const Symbol *s2);

class Symtab;
class Region;
class ExceptionBlock;
class relocationEntry;

const char WILDCARD_CHARACTER = '?';
const char MULTIPLE_WILDCARD_CHARACTER = '*';

/************************************************************************
 * class Object
 *
 *  WHAT IS THIS CLASS????  COMMENTS????
 *  Looks like it has a dictionary hash of symbols, as well as
 *   a ptr to to the code section, an offset into the code section,
 *   and a length of the code section, and ditto for the data
 *   section....
************************************************************************/

class Object : public boost::basic_lockable_adapter<boost::mutex>
{
public:
    DYNINST_EXPORT unsigned nsymbols () const;
    
    DYNINST_EXPORT bool get_symbols( std::string & name, std::vector< Symbol *> & symbols);

    DYNINST_EXPORT char*       code_ptr () const; 
    DYNINST_EXPORT Offset           code_off () const;
    DYNINST_EXPORT Offset           code_len () const;

    DYNINST_EXPORT char*       data_ptr () const;
    DYNINST_EXPORT Offset           data_off () const;
    DYNINST_EXPORT Offset           data_len () const;

    DYNINST_EXPORT bool 	      is_aout  () const;
    DYNINST_EXPORT bool        isDynamic() const;

    DYNINST_EXPORT unsigned	      no_of_sections () const;
    DYNINST_EXPORT unsigned	      no_of_symbols  ()	const;

    DYNINST_EXPORT bool getAllExceptions(std::vector<ExceptionBlock *>&excpBlocks) const;
    DYNINST_EXPORT std::vector<Region *> getAllRegions() const;

    DYNINST_EXPORT Offset loader_off() const;
    DYNINST_EXPORT unsigned loader_len() const;
    DYNINST_EXPORT int getAddressWidth() const;

    bool isStaticBinary() const {return is_static_binary_;}

    DYNINST_EXPORT virtual char *  mem_image() const;

    DYNINST_EXPORT virtual  bool   needs_function_binding()  const;
    DYNINST_EXPORT virtual  bool   get_func_binding_table(std::vector<relocationEntry> &) const;
    DYNINST_EXPORT virtual  bool   get_func_binding_table_ptr(const std::vector<relocationEntry> *&) const; 
    DYNINST_EXPORT virtual  bool   addRelocationEntry(relocationEntry &re);
    DYNINST_EXPORT virtual  bool   getSegments(std::vector<Segment> &) const { return false; }

    DYNINST_EXPORT bool have_deferred_parsing( void ) const;
    // for debuggering....
    DYNINST_EXPORT const std::ostream &dump_state_info(std::ostream &s);

    DYNINST_EXPORT void * getErrFunc() const;
    DYNINST_EXPORT dyn_c_hash_map< std::string, std::vector< Symbol *> > *getAllSymbols();
    
    DYNINST_EXPORT virtual bool hasFrameDebugInfo() {return false;}
    DYNINST_EXPORT virtual bool getRegValueAtFrame(Address /*pc*/,
                                                  Dyninst::MachRegister /*reg*/, 
                                                  Dyninst::MachRegisterVal & /*reg_result*/,
                                                  Dyninst::SymtabAPI::MemRegReader * /*reader*/) {return false;}
    
    DYNINST_EXPORT virtual Dyninst::Architecture getArch() const { return Arch_none; }
    DYNINST_EXPORT bool hasError() const;
    DYNINST_EXPORT virtual bool isBigEndianDataEncoding() const { return false; }
    DYNINST_EXPORT virtual bool getABIVersion(int & /*major*/, int & /*minor*/) const { return false; }


    virtual void setTruncateLinePaths(bool value);
    virtual bool getTruncateLinePaths();
    virtual Region::RegionType getRelType() const { return Region::RT_INVALID; }

    virtual Offset getPreferedBase() const { return 0; }
    virtual bool hasReldyn() const { return false; }
    virtual bool hasReladyn() const { return false; }
    virtual bool hasRelplt() const { return false; }
    virtual bool hasRelaplt() const { return false; }
    virtual bool hasDebugInfo() { return false; }
    virtual void getDependencies(std::vector<std::string> &deps) { deps.clear(); }
    virtual const char *interpreter_name() const { return NULL; }
    virtual Offset getEntryAddress() const = 0;
    virtual Offset getBaseAddress() const = 0;
    virtual Offset getLoadAddress() const = 0;
    virtual bool isEEL() const { return false; }
    virtual void getModuleLanguageInfo(dyn_hash_map<std::string, supportedLanguages> *mod_langs) = 0;

    virtual void insertPrereqLibrary(std::string libname) = 0;
    virtual bool removePrereqLibrary(std::string ) { return false; }
    virtual void insertDynamicEntry(long, long) { }

    virtual Offset getInitAddr() const { return 0; }
    virtual Offset getFiniAddr() const { return 0; }
    virtual Offset getDynamicAddr() const { return 0; }

    virtual void *getElfHandle() { return NULL; }
    virtual LineInformation* parseLineInfoForObject(StringTablePtr) { return NULL; }

    DYNINST_EXPORT virtual bool isOnlyExecutable() const { return false; }
    DYNINST_EXPORT virtual bool isExecutable() const { return false; }
    DYNINST_EXPORT virtual bool isSharedLibrary() const { return false; }
    DYNINST_EXPORT virtual bool isOnlySharedLibrary() const { return false; }
    DYNINST_EXPORT virtual bool isDebugOnly() const { return false; }
    DYNINST_EXPORT virtual bool isLinuxKernelModule() const { return false; }
    DYNINST_EXPORT virtual bool isUnlinkedObjectFile() const { return false; }
    DYNINST_EXPORT virtual bool isPositionIndependent() const { return false; }

    virtual bool convertDebugOffset(Offset, Offset &) { return false; }

    virtual Offset getTOCoffset(Offset) const { return 0; }
    virtual void setTOCoffset(Offset) { }

    virtual bool emitDriver(std::string fName, std::set<Symbol *> &allSymbols, unsigned flag) = 0;
    virtual void parseFileLineInfo() { }
    virtual void parseTypeInfo() { }

    // Only implemented for ELF right now
    DYNINST_EXPORT virtual void getSegmentsSymReader(std::vector<SymSegment> &) {}
	  DYNINST_EXPORT virtual void rebase(Offset) {}
    virtual void addModule(SymtabAPI::Module *) {}

    FileFormat getFileFormat() const { return file_format_; }
protected:
    DYNINST_EXPORT virtual ~Object();
    // explicitly protected
    DYNINST_EXPORT Object(MappedFile *, void (*err_func)(const char *), Symtab*);
friend class Module;
    virtual void parseLineInfoForCU(Offset , LineInformation* ) { }

    MappedFile *mf;

    std::vector< Region *> regions_;

    // XXX symbols_ is the owner of Symbol pointers; memory
    //     is reclaimed from this structure
    dyn_c_hash_map< std::string, std::vector< Symbol *> > symbols_;
    dyn_hash_map< std::string, std::vector< Symbol *> > symbols_tmp_;
    dyn_c_hash_map<Offset, std::vector<Symbol *> > symsByOffset_;
    std::vector<std::pair<std::string, Offset> > modules_;

    char*   code_ptr_;
    Offset code_off_;
    Offset code_len_;

    char*   data_ptr_;
    Offset data_off_;
    Offset data_len_;

    Offset code_vldS_;
    Offset code_vldE_;

    Offset data_vldS_;
    Offset data_vldE_;

    Offset loader_off_;
    Offset loader_len_;

//    Offset loadAddress_;
//    Offset entryAddress_;
//    Offset baseAddress_;
 
    bool is_aout_;
    bool is_dynamic_;
    bool has_error;

    bool is_static_binary_;

    unsigned no_of_sections_;
    unsigned no_of_symbols_;

    bool deferredParse;
    bool parsedAllLineInfo;
    
    void (*err_func_)(const char*);
    int addressWidth_nbytes;

    std::vector<ExceptionBlock> catch_addrs_; //Addresses of C++ try/catch blocks;
    Symtab* associated_symtab;

    FileFormat file_format_;
private:
    friend class SymbolIter;
    friend class Symtab;

    // declared but not implemented; no copying allowed
    Object(const Object &obj);
    const Object& operator=(const Object &obj);
};

}//namepsace Symtab
}//namespace Dyninst

/************************************************************************
 * class SymbolIter
************************************************************************/

namespace Dyninst{
namespace SymtabAPI{

class SymbolIter {
 private:
   dyn_c_hash_map< std::string, std::vector< Symbol *> > *symbols;
   unsigned int currentPositionInVector;
   dyn_c_hash_map< std::string, std::vector< Symbol *> >::iterator symbolIterator;
   
 public:
   SymbolIter( Object & obj );
   SymbolIter( const SymbolIter & src );
   ~SymbolIter ();
   
   void reset ();
   
   operator bool() const;
   void operator++ ( int );
   const std::string & currkey() const;
   
   /* If it's important that this be const, we could try to initialize
      currentVector to '& symbolIterator.currval()' in the constructor. */
   Symbol *currval();
   
 private:	
   
   SymbolIter & operator = ( const SymbolIter & ); // explicitly disallowed
}; /* end class SymbolIter() */

Object *parseObjectFile(MappedFile *, bool, void(*)(const char *) = log_msg, bool = true, Symtab * = NULL);

}//namepsace SymtabAPI
}//namespace Dyninst

#endif /* !defined(_Object_h_) */
