// Generated by Flexc++ V2.08.01 on Mon, 28 Mar 2022 15:38:38 +0530

#ifndef GdiMiLexer_H_INCLUDED_
#define GdiMiLexer_H_INCLUDED_

// $insert baseclass_h
#include "GdiMiLexerbase.h"
#include "GdbMiParserbase.h"

// $insert classHead
class GdiMiLexer: public GdiMiLexerBase
{
    public:
        explicit GdiMiLexer(std::istream &in = std::cin,
                                std::ostream &out = std::cout);

        GdiMiLexer(std::string const &infile, std::string const &outfile);
        
        // $insert lexFunctionDecl
        int lex();

    private:
        int lex_();
        int executeAction_(size_t ruleNr);

        void print();
        void preCode();     // re-implement this function for code that must 
                            // be exec'ed before the patternmatching starts

        void postCode(PostEnum_ type);    
                            // re-implement this function for code that must 
                            // be exec'ed after the rules's actions.
};

// $insert scannerConstructors
inline GdiMiLexer::GdiMiLexer(std::istream &in, std::ostream &out)
:
    GdiMiLexerBase(in, out)
{}

inline GdiMiLexer::GdiMiLexer(std::string const &infile, std::string const &outfile)
:
    GdiMiLexerBase(infile, outfile)
{}

// $insert inlineLexFunction
inline int GdiMiLexer::lex()
{
    return lex_();
}

inline void GdiMiLexer::preCode() 
{
    // optionally replace by your own code
}

inline void GdiMiLexer::postCode([[maybe_unused]] PostEnum_ type) 
{
    // optionally replace by your own code
}

inline void GdiMiLexer::print() 
{
    print_();
}


#endif // GdiMiLexer_H_INCLUDED_

