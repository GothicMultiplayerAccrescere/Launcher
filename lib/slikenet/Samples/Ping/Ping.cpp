/*
 *  Original work: Copyright (c) 2014, Oculus VR, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  RakNet License.txt file in the licenses directory of this source tree. An additional grant 
 *  of patent rights can be found in the RakNet Patents.txt file in the same directory.
 *
 *
 *  Modified work: Copyright (c) 2017, SLikeSoft UG (haftungsbeschränkt)
 *
 *  This source code was modified by SLikeSoft. Modifications are licensed under the MIT-style
 *  license found in the license.txt file in the root directory of this source tree.
 */

// ----------------------------------------------------------------------
// RakNet version 1.0
// Filename Ping.cpp
// Very basic chat engine example
// ----------------------------------------------------------------------
#include "slikenet/MessageIdentifiers.h"

#include "slikenet/peerinterface.h"
#include "slikenet/peerinterface.h"
#include "slikenet/types.h"
#include "slikenet/GetTime.h"
#include "slikenet/BitStream.h"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include "slikenet/Gets.h"
#include "slikenet/linux_adapter.h"
#include "slikenet/osx_adapter.h"

#ifdef WIN32
#include "slikenet/Kbhit.h"
#else
#include "slikenet/Kbhit.h"
#endif

int main(void)
{
	// Pointers to the interfaces of our server and client.
	// Note we can easily have both in the same program
	SLNet::RakPeerInterface *client= SLNet::RakPeerInterface::GetInstance();
	SLNet::RakPeerInterface *server= SLNet::RakPeerInterface::GetInstance();

	int i = server->GetNumberOfAddresses();

	// Holds packets
	SLNet::Packet* p;

	// Record the first client that connects to us so we can pass it to the ping function
	SLNet::SystemAddress clientID= SLNet::UNASSIGNED_SYSTEM_ADDRESS;
	bool packetFromServer;
	char portstring[30];

	printf("This is a sample of how to send offline pings and get offline ping\n");
	printf("responses.\n");
	printf("Difficulty: Beginner\n\n");

	// A server
	puts("Enter the server port to listen on");
	Gets(portstring,sizeof(portstring));
	if (portstring[0]==0)
		strcpy_s(portstring,"60000");

	// Enumeration data
	puts("Enter offline ping response data (for return by a LAN discovery for example)");
	puts("Hit enter for none.");
	char enumData[512];
	Gets(enumData,sizeof(enumData));
	if (enumData[0])
		server->SetOfflinePingResponse(enumData, (const unsigned int) strlen(enumData)+1);

	puts("Starting server.");

	// The server has to be started to respond to pings.
	SLNet::SocketDescriptor socketDescriptor(atoi(portstring),0);
	bool b = server->Startup(2, &socketDescriptor, 1)== SLNet::RAKNET_STARTED;
	server->SetMaximumIncomingConnections(2);
	if (b)
		puts("Server started, waiting for connections.");
	else
	{ 
		puts("Server failed to start.  Terminating.");
		exit(1);
	}

	socketDescriptor.port=0;
	client->Startup(1,&socketDescriptor, 1);

	puts("'q' to quit, any other key to send a ping from the client.");
	char buff[256];

	// Loop for input
	for(;;)
	{
		if (_kbhit())
		{
			// Holds user data
			char ip[64], serverPort[30];

			if (Gets(buff,sizeof(buff))&&(buff[0]=='q'))
				break;
			else
			{

				// Get our input
				puts("Enter IP to ping");
				Gets(ip, sizeof(ip));
				if (ip[0]==0)
					strcpy_s(ip, "127.0.0.1");
				puts("Enter the port to ping");
				Gets(serverPort,sizeof(serverPort));
				if (serverPort[0]==0)
					strcpy_s(serverPort, "60000");

				client->Ping(ip, atoi(serverPort), false);

				puts("Pinging");
			}
		}

		// Get a packet from either the server or the client
		p = server->Receive();

		if (p==0)
		{
			packetFromServer=false;
			p = client->Receive();
		}
		else
			packetFromServer=true;

		if (p==0)
			continue;


		// Check if this is a network message packet
		switch (p->data[0])
		{
			case ID_UNCONNECTED_PONG:
				{
					unsigned int dataLength;
					SLNet::TimeMS time;
					SLNet::BitStream bsIn(p->data,p->length,false);
					bsIn.IgnoreBytes(1);
					bsIn.Read(time);
					dataLength = p->length - sizeof(unsigned char) - sizeof(SLNet::TimeMS);
					printf("ID_UNCONNECTED_PONG from SystemAddress %s.\n", p->systemAddress.ToString(true));
					printf("Time is %i\n",time);
					printf("Ping is %i\n", (unsigned int)(SLNet::GetTimeMS()-time));
					printf("Data is %i bytes long.\n", dataLength);
					if (dataLength > 0)
						printf("Data is %s\n", p->data+sizeof(unsigned char)+sizeof(SLNet::TimeMS));

					// In this sample since the client is not running a game we can save CPU cycles by
					// Stopping the network threads after receiving the pong.
					client->Shutdown(100);
				}
				break;
			case ID_UNCONNECTED_PING:
				break;
			case ID_UNCONNECTED_PING_OPEN_CONNECTIONS:
				break;			
		}

		// We're done with the packet
		if (packetFromServer)
			server->DeallocatePacket(p);
		else
			client->DeallocatePacket(p);
	}

	// We're done with the network
	SLNet::RakPeerInterface::DestroyInstance(server);
	SLNet::RakPeerInterface::DestroyInstance(client);

	return 0;
}
