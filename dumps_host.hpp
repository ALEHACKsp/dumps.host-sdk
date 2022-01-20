#pragma once

#include <Windows.h>
#include <fstream>
#include <string>
#include <filesystem>
#include <sstream>
#include <unordered_map>
#include <wininet.h>

#pragma comment(lib, "wininet.lib")
#define DEBUG_LOGGING true

namespace dumps {

    namespace http {
		std::string replace( std::string subject , const std::string& search ,
			const std::string& replace ) {
			size_t pos = 0;
			while ( ( pos = subject.find( search , pos ) ) != std::string::npos ) {
				subject.replace( pos , search.length( ) , replace );
				pos += replace.length( );
			}
			return subject;
		}

		std::string get( std::string URL ) {
			HINTERNET interwebs = InternetOpenA( "Mozilla/5.0" , INTERNET_OPEN_TYPE_DIRECT , NULL , NULL , NULL );
			HINTERNET urlFile;
			std::string rtn;

			if ( interwebs ) 
			{
				urlFile = InternetOpenUrlA( interwebs , URL.c_str( ) , NULL , NULL , NULL , NULL );
				if ( urlFile ) 
				{
					char buffer[2000];
					DWORD bytesRead;
					
					do 
					{
						InternetReadFile( urlFile , buffer , 2000 , &bytesRead );
						rtn.append( buffer , bytesRead );
						memset( buffer , 0 , 2000 );
					} while ( bytesRead );

					InternetCloseHandle( interwebs );
					InternetCloseHandle( urlFile );
					std::string p = http::replace( rtn , "|n" , "\r\n" );
					return p;
				}
			}
			InternetCloseHandle( interwebs );
			std::string p = http::replace( rtn , "|n" , "\r\n" );

			return p;
		}
    }

	std::string format( const char* buffer , ... ) {
		char buf[4096];
		va_list args;
		va_start( args , buffer );
		_vsnprintf_s( buf , sizeof(buf) , buffer , args );
		va_end( args );

		return buf;
	}

	inline std::string to_hex( int value ) {
		std::stringstream string;
		string << "0x" << std::hex << std::uppercase << value;
		return string.str( );	
	}

	template<typename type = int>
	inline std::string json_value( std::string key , type value ) {
		return format( " \"%s\":\"%s\"\n" , key.c_str() , to_hex(value) );
	}

	inline bool is_valid_string( std::string& buffer ) {
		return ( buffer.length( ) > 1 && buffer != "" && (&buffer) != nullptr );
	}
	
	inline void make_dump_file( std::string name, std::string buffer ) {
		auto current = std::filesystem::current_path( );
		auto path = current / name;

		std::ofstream dump_file( path );

		dump_file << "{\n";
		dump_file << buffer;
		dump_file << "\n}\n";

		dump_file.close( );

		ShellExecuteA( 0 , "open" , path.string( ).c_str( ) , 0 , 0 , SW_SHOW );
	}

	enum game : int {
		csgo = 0,
		apex_legends = 1,
		rocket_league = 2,
		paladins = 3,
		realm_royale = 4,
		black_sqaud = 5,
		super_people = 6,
		ready_or_not = 7,
		age_of_ashes = 8,
		propnight = 9,
		ark_survival = 10,
		conan_exiles = 11,
		myth_of_empires = 12,
		splitgate = 13,
		rogue_company = 14,
		black_4_blood = 15,
		hell_let_loose = 16,
		dead_by_daylight = 17,
		scum = 18,
		squad = 19,
		sea_of_thieves = 20,
		bloodhunt = 21,
		deadside = 22,
		cycle_frontier = 23,
		mir4 = 24,
		promod = 25,
		dysterra = 26,
		rust = 27,
		fallguys = 28,
		zero_hour = 29
	};

	enum game_type : int {
		unreal_engine = 0, /*unreal engine 4+*/
		source_engine = 1, /*csgo, apex legends, etc*/
		unity_engine  = 2  /*il2cpp*/
	};

	inline std::unordered_map<dumps::game , std::pair<std::string, dumps::game_type>> games = {
		{ game::csgo, {"csgo", game_type::source_engine } },
		{ game::apex_legends, {"apex", game_type::source_engine } },
		{ game::rocket_league, { "rl", game_type::unreal_engine } },
		{ game::paladins, { "paladins", game_type::unreal_engine } },
		{ game::realm_royale, { "realm", game_type::unreal_engine } },
		{ game::black_sqaud, { "bs", game_type::unreal_engine } },
		{ game::super_people, { "sp", game_type::unreal_engine } },
		{ game::ready_or_not, { "ron", game_type::unreal_engine } },
		{ game::age_of_ashes, { "century", game_type::unreal_engine }},
		{ game::propnight, { "pn", game_type::unreal_engine } },
		{ game::ark_survival, { "ark", game_type::unreal_engine } },
		{ game::conan_exiles, { "ce", game_type::unreal_engine } },
		{ game::myth_of_empires, { "moe", game_type::unreal_engine } },
		{ game::splitgate, { "sg", game_type::unreal_engine } },
		{ game::rogue_company, { "rc", game_type::unreal_engine } },
		{ game::black_4_blood, { "b4b", game_type::unreal_engine } },
		{ game::hell_let_loose, { "hll", game_type::unreal_engine } },
		{ game::dead_by_daylight, { "dbd" , game_type::unreal_engine } },
		{ game::scum, { "scum", game_type::unreal_engine } },
		{ game::squad, { "squad", game_type::unreal_engine } },
		{ game::sea_of_thieves, { "sot" , game_type::unreal_engine } },
		{ game::bloodhunt, { "bh", game_type::unreal_engine } },
		{ game::deadside, { "ds", game_type::unreal_engine } },
		{ game::cycle_frontier, { "tcf" , game_type::unreal_engine } } ,
		{ game::mir4, { "mir4", game_type::unreal_engine } },
		{ game::promod, {"promod", game_type::unreal_engine } },
		{ game::dysterra, { "dysterra", game_type::unreal_engine } },
		{ game::rust, { "rust", game_type::unity_engine } },
		{ game::fallguys, {"fg", game_type::unity_engine } },
		{ game::zero_hour, { "zh", game_type::unity_engine } }
	};

	struct game_data_structure {

		std::string name;
		bool        is_dumped;
		game_type   type;

		// unreal_engine
		uintptr_t   g_world;
		uintptr_t   g_objects;
		uintptr_t   g_names;

		// source_engine
		uintptr_t  g_client_state;

		bool is_valid( ) {
			if ( type == dumps::game_type::unreal_engine )
				return ( g_world != 0 && g_names != 0 && g_objects != 0 );
			else if ( type == dumps::game_type::source_engine )
				return ( g_client_state != 0 );
			else
				return is_dumped ? true : false; // unity is not supported yet (kinda.., there's no offsets page)
		}
	};

	inline game_data_structure* game_data = new game_data_structure( );

	inline game_data_structure* dump_unreal_engine( const std::string api_key , const std::string name , bool save_to_file = false ) {

		if ( name.length( ) <= 0 || api_key.length( ) <= 0 )
		{
			return nullptr;
		}

		game_data->type = game_type::unreal_engine;

#if DEBUG_LOGGING
		printf( "[+] Dumping (https://%s.dumps.host), this may take some time..\n" , name.c_str( ) );
#endif

		auto gworld_url = dumps::format( "https://%s.dumps.host/api/offset?key=%s&name=GWorld" , name.c_str( ) , api_key.c_str( ) );
		auto gnames_url = dumps::format( "https://%s.dumps.host/api/offset?key=%s&name=GNames" , name.c_str( ) , api_key.c_str( ) );
		auto gobjects_url = dumps::format( "https://%s.dumps.host/api/offset?key=%s&name=GObjects" , name.c_str( ) , api_key.c_str( ) );

		auto gworld_data = dumps::http::get( gworld_url );
		auto gnames_data = dumps::http::get( gnames_url );
		auto gobjects_data = dumps::http::get( gobjects_url );

#if DEBUG_LOGGING
		if ( is_valid_string( gworld_data ) ) printf( "[+] Dumped GWorld (0x%llx)\n" , std::atoi( gworld_data.c_str( ) ) );
		if ( is_valid_string( gnames_data ) ) printf( "[+] Dumped GNames (0x%llx)\n" , std::atoi( gnames_data.c_str( ) ) );
		if ( is_valid_string( gobjects_data ) ) printf( "[+] Dumped GObjects (0x%llx)\n" , std::atoi( gobjects_data.c_str( ) ) );
#endif

		game_data->name = name;
		game_data->g_world = std::atoi( gworld_data.c_str( ) );
		game_data->g_names = std::atoi( gnames_data.c_str( ) );
		game_data->g_objects = std::atoi( gobjects_data.c_str( ) );
		game_data->is_dumped = true;

		if ( save_to_file )
		{
			dumps::make_dump_file( "unreal_engine_dump.json" ,
				dumps::json_value( "GWorld" , game_data->g_world ) + "," +
				dumps::json_value( "GNames" , game_data->g_names ) + "," +
				dumps::json_value( "GObjects" , game_data->g_objects )
			);
		}

#if DEBUG_LOGGING
		printf( "\nExample how to use:\n\n" );

		printf( "dumps::dump_unreal_engine(\"api_key\", \"sg\", false);\n" );
		printf( "std::uintptr_t owning_game_instance = *(std::uintptr_t*)(dumps::game_data->g_world + 0x180);\n=========================================\n" );
#endif

		return game_data;
	}

	inline game_data_structure* dump_source_engine( const std::string api_key , const std::string name , bool save_to_file = false ) {

		if ( name.length( ) <= 0 || api_key.length( ) <= 0 )
		{
			return nullptr;
		}

		game_data->type = game_type::source_engine;

#if DEBUG_LOGGING
		printf( "[+] Dumping (https://%s.dumps.host), this may take some time..\n" , name.c_str( ) );
#endif

		auto client_state_url = dumps::format( "https://%s.dumps.host/api/offset?key=%s&name=client_state" , name.c_str( ) , api_key.c_str( ) );
		auto client_state_data = dumps::http::get( client_state_url );

#if DEBUG_LOGGING
		if ( is_valid_string( client_state_data ) ) printf( "[+] client_state: 0x%llx\n" , std::atoi( client_state_data.c_str( ) ) );
#endif
		game_data->g_client_state = std::atoi( client_state_data.c_str( ) );
		game_data->is_dumped = true;

		if ( save_to_file )
		{
			dumps::make_dump_file( "source_engine_dump.json" ,
				dumps::json_value( "client_state" , game_data->g_client_state )
			);
		}

#if DEBUG_LOGGING
		printf( "\nExample how to use:\n\n" );

		printf( "dumps::dump_source_engine(\"api_key\", \"csgo\", false);\n" );
		printf( "std::uintptr_t client_state = ((std::uintptr_t)GetModuleHandleA(0) + 0x%llx);\n=========================================\n", game_data->g_client_state );
#endif

		return game_data;
	}
	
	inline game_data_structure* dump_unity_engine( const std::string api_key , const std::string name , bool save_to_file = false ) {

		if ( name.length( ) <= 0 || api_key.length( ) <= 0 )
		{
			return nullptr;
		}

		game_data->type = game_type::unity_engine;

#if DEBUG_LOGGING
		printf( "[+] Dumping (https://%s.dumps.host), this may take some time..\n" , name.c_str( ) );
#endif

		// Replace with your own things you'd like to dump.
		// You can use these links for reference: https://fg.dumps.host/api/, https://rust.dumps.host/api/, https://zh.dumps.host/api/

		auto url = dumps::format( "https://%s.dumps.host/api/offset?key=%s&name=SecurityParser&member=current" , name.c_str( ) , api_key.c_str( ) );
		auto data = dumps::http::get( url );

		printf( "[+] SecurityParser.current: 0x%llx\n" , std::atoi( data.c_str( ) ) );

		game_data->is_dumped = true;

		if ( save_to_file )
		{
			dumps::make_dump_file( "unity_engine_dump.json" ,
				dumps::json_value( "SecurityParser.current" , std::atoi( data.c_str( ) ) )
			);
		}

		return game_data;
	}

	inline game_data_structure* dump_game( const std::string api_key, dumps::game game_to_dump , bool save_to_file ) {
		
		if ( api_key.length( ) <= 0 )
		{
			return nullptr;
		}

		dumps::game      to_dump = { };
		dumps::game_type type	 = { };
		std::string      name    = { };

		for (auto& [key, value] : dumps::games)
			if ( key == game_to_dump )
			{
				to_dump = key;
				name    = value.first;
				type    = value.second;

				break;
			}

		if ( name.length( ) <= 0 )
		{
			return nullptr;
		}

		switch ( type ) {
		case dumps::game_type::unreal_engine:
			dumps::dump_unreal_engine( api_key , name , save_to_file );
			break;

		case dumps::game_type::source_engine:
			dumps::dump_source_engine( api_key , name , save_to_file );
			break;

		case dumps::game_type::unity_engine:
			dumps::dump_unity_engine( api_key , name , save_to_file );
			break;
		}

		return game_data;
	}
}
