//
//  kaniCmdLine.h
//  KaniTexTool
//
//  Created by Christian Helmich on 14.05.11.
//  Copyright 2011 KageKirin. All rights reserved.
//

#pragma once
#ifndef KANI_CMDLINE_H
#define KANI_CMDLINE_H	1

#include <string>

namespace boost { namespace program_options {

	class options_description;
	class variables_map;
	
}}

namespace kani { namespace cmd {

	namespace bpo = boost::program_options;
	using std::string;
	
	class CmdLineOptions
	{
	private:
		int		m_state;
		string	m_format;	//TODO: replace by internal enum later
		string	m_converter;
		string	m_inputFile;
		string	m_outputFile;
		int		m_mipmaps;
		bool	m_regenMips;
	
	
	public:
		CmdLineOptions(int argc, const char** argv);
		
		int getState() const;
		const string& getInputFile() const;
		const string& getOutputFile() const;
		int getMipmaps() const;
		bool getRegenMips() const;
		const string& getFormat() const;	//TODO: enum
		const string& getConverter() const;
	
	private:
		int parse(int argc, const char** argv);
		void buildDescription(bpo::options_description& desc);
		int mapVariables(bpo::variables_map& vm);
	};
	
}}

#endif	//KANI_CMDLINE_H
