//
//  kaniCmdLine.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 14.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

//#include "stdafx.h"
#include "kaniCmdLine.h"
#include "../texture/kaniTexFormat.h"

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <boost/program_options.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/stringize.hpp>

#ifdef _WIN32
#	ifdef _DEBUG
#		pragma comment(lib, "libboost_program_options-vc90-mt-sgd-1_46_1.lib") 
#	else
#		pragma comment(lib, "libboost_program_options-vc90-mt-s-1_46_1.lib") 
#	endif
#endif

namespace kani { namespace cmd {

	using std::vector;
	using std::string;
	using std::invalid_argument;

	CmdLineOptions::CmdLineOptions(int argc, const char** argv):
	m_mipmaps(-1),//means same as input
	m_regenMips(false)
	{
		m_state = parse(argc, argv);
	}
	
	int CmdLineOptions::parse(int argc, const char** argv)
	{
		bpo::options_description desc("Allowed options");
		buildDescription(desc);
	
		bpo::positional_options_description pop;
		pop.add("input-file", 1);
		pop.add("output-file", 1);

		bpo::variables_map vm;
		bpo::store(bpo::command_line_parser(argc, argv).
				   options(desc).positional(pop).run(),
				   vm);
		bpo::notify(vm);
	
		if (vm.count("help"))
		{
			std::cout << desc << "\n";
			return 1;
		}
		return mapVariables(vm);	
	}
	
	
#define M_CREATEDEF(r, data, elem)	\
	BOOST_PP_STRINGIZE(SEXTUPLE_1ST(elem)) ", "

#define FIRST_ELEM		BOOST_PP_SEQ_ELEM(0, FORMAT_SEQ)
#define FIRST_ELEM_S	BOOST_PP_STRINGIZE(FIRST_ELEM)
		
	void CmdLineOptions::buildDescription(bpo::options_description& desc)
	{
		//NOTE: what kind of DSL (domain-specific language) is this?
		desc.add_options()
			("help,h", "print this message")
			("format,f", bpo::value<string>()->default_value(FIRST_ELEM_S), "set output format as one of the following: "
				BOOST_PP_SEQ_FOR_EACH(M_CREATEDEF, 0, FORMAT_SEQ)
			)
			("mipmaps,m", bpo::value<int>(), "set output mipmap count. 0 for no mipmaps")
			("no-gen-mipmaps", bpo::value<int>()->implicit_value(1), "do not generate mipmaps")
			("regen-mipmaps", bpo::value<int>()->implicit_value(1), "re-generate mipmaps. i.e. do not use mips from input texture")
			("converter,c", bpo::value<string>()->default_value("pvr"), "set converter")
			//TODO: expand here
		
			//finally
			("input-file", bpo::value<string>(), "input file")
			("output-file", bpo::value<string>(), "output file")			
		;
	}
	
	int CmdLineOptions::mapVariables(bpo::variables_map& vm)
	{	
		if(vm.count("format"))
		{
			std::cout << "format set to " 
				<< vm["format"].as<string>() << ".\n";
			m_format = vm["format"].as<string>();
		}
		else
		{
			std::cout << "no format set. transcoding only.\n";
		}
		
		if(vm.count("converter"))
		{
			std::cout << "converter set to " 
				<< vm["converter"].as<string>() << ".\n";
			m_converter = vm["converter"].as<string>();
		}
		else
		{
			std::cout << "no converter set. defaulting to pvr.\n";
			m_converter = "pvr";
		}
	
		if(vm.count("mipmaps"))
		{
			std::cout << "mipmaps set to " 
				<< vm["mipmaps"].as<int>() << ".\n";
			m_mipmaps = vm["mipmaps"].as<int>();
		}
		
		if(vm.count("no-gen-mipmaps"))
		{
			std::cout << "no-gen-mipmaps set to " 
				<< vm["no-gen-mipmaps"].as<int>() << ".\n";
		}

		if(vm.count("regen-mipmaps"))
		{
			std::cout << "regen-mipmaps set to " 
				<< vm["regen-mipmaps"].as<int>() << ".\n";
			m_regenMips = vm["regen-mipmaps"].as<bool>();
		}
		
		if(vm.count("input-file"))
		{
			std::cout << "input-file set to " 
				<< vm["input-file"].as<string>() << ".\n";
			m_inputFile = vm["input-file"].as<string>();
		}
		
		if(vm.count("output-file"))
		{
			std::cout << "output-file set to " 
				<< vm["output-file"].as<string>() << ".\n";
			m_outputFile = vm["output-file"].as<string>();
		}
		else
		{
			m_outputFile = m_inputFile;
			m_outputFile.insert(m_outputFile.find_last_of('.'), "_out");
		}
	
		//get options
		return 0;

	}
	
	//----------------------------------------------------------------------
	
	int CmdLineOptions::getState() const
	{
		return m_state;
	}
	
	const string& CmdLineOptions::getInputFile() const
	{
		return m_inputFile;
	}
	
	const string& CmdLineOptions::getOutputFile() const
	{
		return m_outputFile;
	}
	
	int CmdLineOptions::getMipmaps() const
	{
		return m_mipmaps;
	}
	
	bool CmdLineOptions::getRegenMips() const
	{
		return m_regenMips;
	}
	
	const string& CmdLineOptions::getFormat() const
	{
		return m_format;	
	}

	const string& CmdLineOptions::getConverter() const
	{
		return m_converter;
	}
	
	//----------------------------------------------------------------------
	
}}




