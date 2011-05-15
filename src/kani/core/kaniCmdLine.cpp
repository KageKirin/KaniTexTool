//
//  kaniCmdLine.cpp
//  KaniTexTool
//
//  Created by Christian Helmich on 14.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#include "kaniCmdLine.h"
#include <vector>
#include <string>
#include <stdexcept>
#include <boost/program_options.hpp>


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
		
	void CmdLineOptions::buildDescription(bpo::options_description& desc)
	{
		//NOTE: what kind of DSL (domain-specific language) is this?
		desc.add_options()
			("help,h", "print this message")
			("format,f", bpo::value<string>(), "set output format")
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
	
	int CmdLineOptions::getState()
	{
		return m_state;
	}
	
	const string& CmdLineOptions::getInputFile()
	{
		return m_inputFile;
	}
	
	const string& CmdLineOptions::getOutputFile()
	{
		return m_outputFile;
	}
	
	int CmdLineOptions::getMipmaps()
	{
		return m_mipmaps;
	}
	
	bool CmdLineOptions::getRegenMips()
	{
		return m_regenMips;
	}
	
	const string& CmdLineOptions::getFormat()
	{
		return m_format;	
	}
	
	//----------------------------------------------------------------------
	
}}




