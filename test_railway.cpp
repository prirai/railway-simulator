#include "lib/railway_lib.h"

#include <cassert>		// for assert macro
#include <stdexcept>		// for std::exception
#include <iostream>
#include <fstream>
using namespace std;

vector < string > readTokens (const string & line)
{
  istringstream iss (line);
  vector < string > tokens;
  string token;

  while (getline (iss, token, ','))
    {
      token.erase (0, token.find_first_not_of (" \t"));
      token.erase (token.find_last_not_of (" \t") + 1);
      tokens.push_back (token);
    }

  return tokens;
}

bool
is_number (const std::string & s)
{
  return !s.empty () && std::find_if (s.begin (), s.end (),[](unsigned char c)
				      { return !std::isdigit (c);
				      }
  ) == s.end ();
}

void
file_input ()
{
  vector < Station < int >>int_stations;
  vector < Station < string > >str_stations;
  ifstream ifile;
  StationBase *st_ptr;

  vector < string > command_types = { "s", "alp", "at", "stat", "chs" };
  ifile.open ("input.txt");
  if (!ifile.is_open ())
    {
      cerr << "Error opening input file" << endl;
      exit (1);
    }
  string line;
  int lineindex = 0;
  int station_index = 0;
  int str_station = 0;
  while (getline (ifile, line))
    {
      // cout << line << endl;
      lineindex++;
      auto tokens = readTokens (line);
      int cmd_ind =
	find (command_types.begin (), command_types.end (),
	      tokens[0]) - command_types.begin ();
      // cout << cmd_ind << endl;
      switch (cmd_ind)
	{
	case 0:
	  {
	    // cout << "Trying to add station: " << tokens[1] << endl;
	    bool isnum = is_number (tokens[1]);
	    bool errorocc = true;
	    if (isnum)
	      {
		int int_station = atoi (tokens[1].c_str ());
		if (is_number (tokens[2].c_str ())
		    && atoi (tokens[2].c_str ()) > 0)
		  {
		    auto *temp_st =
		      new Station < int >(int_station,
					  atoi (tokens[2].c_str ()));
		    int_stations.push_back (*temp_st);
		    str_station = 0;
		    station_index = int_stations.size () - 1;
		    st_ptr = &int_stations[station_index];
		    errorocc = false;
		  }
	      }
	    else
	      {
		if (is_number (tokens[2].c_str ())
		    && atoi (tokens[2].c_str ()) > 0)
		  {
		    Station < string > *temp_st =
		      new Station < string > (tokens[1],
					      atoi (tokens[2].c_str ()));
		    str_stations.push_back (*temp_st);
		    str_station = 1;
		    station_index = str_stations.size () - 1;
		    st_ptr = &str_stations[station_index];
		    errorocc = false;
		  }
	      }
	    if (errorocc)
	      {
		std::stringstream errorStream;
		errorStream << "Number of platforms [" << tokens[2] <<
		  "] should be a positive integer";
		printError (errorStream.str ());
		continue;
	      }
	  }
	  break;
	case 1:
	  {
	    string line_name = tokens[1];
	    int platform_id = atoi (tokens[2].c_str ());
	    // cout << "Adding platform " << platform_id << endl;
	    st_ptr->addLineAndPlatform (line_name, platform_id);
	  }
	  break;
	case 2:
	  {
	    int platform_id = atoi (tokens[1].c_str ());
	    string time = tokens[2].c_str ();
	    int time_int = atoi (tokens[2].c_str ());
	    if (is_number (time) && time_int >= 0 && time_int < 2400)
	      {
		bool is_through =
		  (tokens[3].c_str () == "true") ? true : false;
		st_ptr->addTrain (platform_id, time_int, is_through);
	      }
	    else
	      {
		std::stringstream errorStream;
		errorStream << "Time entered [" << tokens[2] <<
		  "] is invalid and should be between 0000 and 2400 hrs.";
		printError (errorStream.str ());
	      }
	  }
	  break;
	case 3:
	  {
	    st_ptr->stationStats ();
	  }
	  break;
	case 4:
	  {
	    bool is_num = is_number (tokens[1]);
	    bool st_found = false;
	    if (is_num)
	      {
		int int_station = atoi (tokens[1].c_str ());
	      for (auto & station:int_stations)
		  {
		    if (station.get_station () == int_station)
		      {
			st_ptr = &station;
			st_found = true;
			break;
		      }
		  }
		if (!st_found)
		  {
		    std::stringstream errorStream;
		    errorStream << "Station not found - " << int_station;
		    printError (errorStream.str ());
		  }
	      }
	    else
	      {
		string str_station = tokens[1];
	      for (auto & station:str_stations)
		  {
		    if (station.get_station () == str_station)
		      {
			st_ptr = &station;
			st_found = true;
			break;
		      }
		  }
		if (!st_found)
		  {
		    cerr << "Station not found - " << str_station << endl;
		  }
	      }
	  }
	  break;
	default:
	  {
	    cout << "Something unusual in input line " << lineindex << endl;
	  }
	}
      cout << "Current Station : ";
      st_ptr->printStationID ();
    }
  exit (0);
}

int
main ()
{
  file_input ();

  try
  {
    int id = 10;
    assert (id > 0 && "Invalid station ID");
    int numPlatforms = 2;
    assert (numPlatforms > 0 && "Invalid number of platforms");
    Station st1 (id, numPlatforms);

    std::string direction = "South";
    assert (!direction.empty () && "Invalid direction");
    int platform = 1;
    assert (platform > 0 && "Invalid platform number");
    st1.addLineAndPlatform (direction, platform);

    platform = 1;
    assert (platform > 0 && "Invalid train platform");
    int time = 100;
    assert (time >= 0 && time < 2400 && "Invalid train time");
    bool arriving = true;
    assert (arriving == true || arriving == false
	    && "Invalid train arrival status");
    st1.addTrain (platform, time, arriving);

    direction = "NW";
    assert (!direction.empty () && "Invalid direction");
    platform = 2;
    assert (platform > 0 && "Invalid platform number");
    st1.addLineAndPlatform (direction, platform);

    platform = 1;
    assert (platform > 0 && "Invalid train platform");
    time = 200;
    assert (time >= 0 && time < 2400 && "Invalid train time");
    arriving = true;
    assert (arriving == true || arriving == false
	    && "Invalid train arrival status");
    st1.addTrain (platform, time, arriving);

    platform = 1;
    assert (platform > 0 && "Invalid train platform");
    time = 120;
    assert (time >= 0 && time < 2400 && "Invalid train time");
    arriving = false;
    assert (arriving == true || arriving == false
	    && "Invalid train arrival status");
    st1.addTrain (platform, time, arriving);

    platform = 1;
    assert (platform > 0 && "Invalid train platform");
    time = 130;
    assert (time >= 0 && time < 2400 && "Invalid train time");
    arriving = true;
    assert (arriving == true || arriving == false
	    && "Invalid train arrival status");
    st1.addTrain (platform, time, arriving);

    platform = 2;
    assert (platform > 0 && "Invalid train platform");
    time = 190;
    assert (time >= 0 && time < 2400 && "Invalid train time");
    arriving = false;
    assert (arriving == true || arriving == false
	    && "Invalid train arrival status");
    st1.addTrain (platform, time, arriving);

    st1.stationStats ();
  } catch (const std::exception & e)
  {
    std::cerr << "Exception: " << e.what () << '\n';
  }
}
