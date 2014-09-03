#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <version.h>
#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <boost/asio/ip/tcp.hpp>


static const std::string	HTTP_SERVER	=	"54.164.6.197";
static const std::string	URL_PATH	=	"/~ubuntu/";
static const std::string	VERSION_URL	=	"version.html";
static const std::string	FILELIST_NAME	=	"filelist.lst";

static const int DISPLAY_VERSION =
                           1000000 * DISPLAY_VERSION_MAJOR
                         +   10000 * DISPLAY_VERSION_MINOR
                         +     100 * DISPLAY_VERSION_REVISION
                         +       1 * DISPLAY_VERSION_BUILD;

using namespace std;
using namespace boost;
using namespace boost::asio;
using boost::asio::ip::tcp;

int getWebVersion ()
{

	boost::asio::ip::tcp::iostream webStream;
	// add a timeout
	webStream.expires_from_now(boost::posix_time::seconds(15));
	webStream.connect(HTTP_SERVER, "http");
	if(!webStream)
	{
		printf("Unable to connect to %s   Reason %s\n",(char*)HTTP_SERVER.c_str(),(char*)webStream.error().message().c_str());
		return 1;
	}
	webStream << "GET " << URL_PATH << VERSION_URL << " HTTP/1.\r\n";
	webStream << "Host: " << HTTP_SERVER << "\r\n";
	webStream << "Accept: */*\r\n"; /**/

	webStream << "Connection: close\r\n\r\n";

	std::string http_version;
	webStream >> http_version;
	unsigned int status_code;
	webStream >> status_code;
	std::string status_message;
	std::getline(webStream, status_message);
	if (!webStream || http_version.substr(0,5)  != "HTTP/")
	{
		printf("Invalid response\n");
		return 2;
	}
	if (status_code != 200)
	{
		printf("Response returned with status code %d\n",status_code);
	}

	std::string header;
	while(std::getline(webStream, header) && header != "\r")
	{//grind though the headers I am sure there is a better way
	}

	std::ostringstream clientVersion;
	clientVersion << webStream.rdbuf();

//	printf("Version %s\n",(char*)clientVersion.str().c_str());
	unsigned int returnVer;
	returnVer = atoi((char*)clientVersion.str().c_str());
//	printf("%d",ver);
	return returnVer;
}

void removeBlockchain() 
{// basically a copy from txdb-leveldb.cpp init_blockindex

        boost::filesystem::path directory = GetDataDir() / "txleveldb";
        boost::filesystem::remove_all(directory);
        unsigned int nFile = 1;
        while (true)
        {
                boost::filesystem::path strBlockFile = GetDataDir() / strprintf("blk%04u.dat", nFile);
                if(!boost::filesystem::exists( strBlockFile))
                        break;
                boost::filesystem::remove(strBlockFile);
                nFile++;
        }

        boost::filesystem::create_directory(GetDataDir() / "txleveldb");
}

int downloadFile(	const char * getFilename, //what were going to save the files as
			const std::string& serverName,
			const std::string& getCommand,
			int currentFileNumber,
			int totalFileNumber)
{
  	boost::asio::io_service io_service;

	// Get a list of endpoints corresponding to the server name.
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(serverName, "http");
	tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	tcp::resolver::iterator end;

  	// Try each endpoint until we successfully establish a connection.
  	tcp::socket socket(io_service);
  	boost::system::error_code error = boost::asio::error::host_not_found;
  	while (error && endpoint_iterator != end)
  	{
		socket.close();
		socket.connect(*endpoint_iterator++, error);
	}

	boost::asio::streambuf request;
	std::ostream request_stream(&request);

	request_stream << "GET " << URL_PATH << getCommand << " HTTP/1.0\r\n";
	request_stream << "Host: " << serverName << "\r\n";
	request_stream << "Accept: */*\r\n";
	request_stream << "Connection: close\r\n\r\n";


	// Send the request.
	boost::asio::write(socket, request);

	// Read the response status line.
	boost::asio::streambuf response;
	boost::asio::read_until(socket, response, "\r\n");

	// Check that response is OK.
	std::istream response_stream(&response);
	std::string http_version;
	response_stream >> http_version;
	unsigned int status_code;
	response_stream >> status_code;
	if (http_version.substr(0,5)  != "HTTP/")
        {
                printf("Invalid response\n");
                return 2;
        }
        if (status_code != 200)
        {
                printf("Response returned with status code %d\n",status_code);
        }

	std::string status_message;
	std::getline(response_stream, status_message);


	// Read the response headers, which are terminated by a blank line.
	boost::asio::read_until(socket, response, "\r\n\r\n");

	// Process the response headers.
	std::string header;
	unsigned int fileSize=0;
	unsigned int currentDLSize;
	float currentPer;
	char mOut[256];
	unsigned int progress;
	filesystem::path workingDir = GetDataDir();
	std::string workingPath = workingDir.string();
	workingPath += "/";
	workingPath += getFilename;

	printf("Downloading File: %s (%d of %d)\n",(char*)workingPath.c_str(),currentFileNumber,totalFileNumber);

	std::ofstream outFile((char*)workingPath.c_str(), std::ofstream::out | std::ofstream::binary);
	while (std::getline(response_stream, header) && header != "\r")
	{
		if(header.find("Content-Length") != std::string::npos)
		{
			fileSize = atoi(header.substr(16).c_str());
		}
	}
	// Write whatever content we already have to output.
	if (response.size() > 0)
	{
		outFile << &response;
	}
	// Read until EOF, writing data to output as we go.
	while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
	{
		outFile << &response;
		currentDLSize =  outFile.tellp();
		currentPer = ((float)currentDLSize / fileSize) * 100;
		progress++;
		if (progress >= 128) //rate limit gui update of progress
		{
//			std::cout << currentDLSize << " of " << fileSize << " " << currentPer <<"%\n";
			progress = 0;
//			sprintf (mOut , "Downloading File:%s (%d of %d)  %d K of %d K  %5.2f%%\r",getFilename,currentFileNumber,totalFileNumber,(currentDLSize / 1024),(fileSize / 1024),currentPer);
			sprintf (mOut , "Downloading File:%s (%d of %d)  %d K of %d K  %d%%\r",getFilename,currentFileNumber,totalFileNumber,(currentDLSize / 1024),(fileSize / 1024),(int)currentPer);
//			printf ("%s",mOut);
			uiInterface.InitMessage(mOut);
		}
	}
outFile.close();
return 0;
}

void processFilelist()
{
   filesystem::path workingDir = GetDataDir();
   std::string workingPath = workingDir.string();
   std::string fileList = workingPath;
   workingPath += "/";
   fileList += "/filelist.lst";

        string myArray[128];
        int i=0;
        using namespace std;
        ifstream file((char*)fileList.c_str());
        if(file.is_open())
        {
                while(!file.eof())
                {
                        file >> myArray[i];
                        ++i;
                }
        }
	printf("%d Files to download\n",i);
        for(int a=0;a<i;++a)
        {
workingPath += myArray[a];
//printf ("File %d of %d \n",a,i);
downloadFile((char*)myArray[a].c_str(),HTTP_SERVER,(char*)myArray[a].c_str(),(a+1),i);
        }

}

int firstRunCheck ()
{//check to see if both items exist if not delete them and download it directly
unsigned int nFile = 1;
filesystem::path directory = GetDataDir() / "txleveldb";
filesystem::path strBlockFile = GetDataDir() / strprintf("blk%04u.dat", nFile);
if (!filesystem::exists(directory) || !filesystem::exists(strBlockFile))
{
	printf("Missing or incomplete blockchain files detected, re-download started\n");
	return 0;
	} else {
	return 1;
}

}


void downloadAndReplaceBlockchain()
{
        downloadFile("filelist.lst",HTTP_SERVER,"/blocklist.lst",1,1);
        int64 sDownload;
        sDownload = GetTimeMillis();
        removeBlockchain();     //this removes all the block chain from .fluttercoin dir
                                //blk????.dat and txleveldb dir
        processFilelist();      //reads the filelist and downloads all files in the list
        printf("Download      %15"PRI64d"ms\n", GetTimeMillis() - sDownload);
}

