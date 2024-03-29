/*
 *  Original work: Copyright (c) 2014, Oculus VR, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  RakNet License.txt file in the licenses directory of this source tree. An additional grant 
 *  of patent rights can be found in the RakNet Patents.txt file in the same directory.
 *
 *
 *  Modified work: Copyright (c) 2016-2017, SLikeSoft UG (haftungsbeschränkt)
 *
 *  This source code was modified by SLikeSoft. Modifications are licensed under the MIT-style
 *  license found in the license.txt file in the root directory of this source tree.
 */

#include "slikenet/peerinterface.h"

#include "slikenet/MessageFilter.h"
#include "slikenet/MessageIdentifiers.h"
#include "slikenet/sleep.h"
#include <stdio.h>

int main()
{
	// The message filter parses all incoming messages and only allows messages of a certain type
	SLNet::MessageFilter messageFilter;
	SLNet::RakPeerInterface *peer1, *peer2;

	char message;
	SLNet::Packet *packet;
	peer1= SLNet::RakPeerInterface::GetInstance();
	peer2= SLNet::RakPeerInterface::GetInstance();

	// Set up the filter rules.
	// All new connections go to filter 0
	messageFilter.SetAutoAddNewConnectionsToFilter(0);
	// Filter 0 only allows ID_USER_PACKET_ENUM
	messageFilter.SetAllowMessageID(true, ID_USER_PACKET_ENUM, ID_USER_PACKET_ENUM, 0);
	// Filter 1 only allows ID_USER_PACKET_ENUM
	messageFilter.SetAllowMessageID(true, ID_USER_PACKET_ENUM+1, ID_USER_PACKET_ENUM+1, 1);
	// Use the filter on peer 1.
	peer1->AttachPlugin(&messageFilter);

	// Connect the systems to each other
	SLNet::SocketDescriptor socketDescriptor(60000,0);
	peer1->Startup(1,&socketDescriptor, 1);
	peer1->SetMaximumIncomingConnections(1);
	socketDescriptor.port=60001;
	peer2->Startup(1,&socketDescriptor, 1);
	peer2->Connect("127.0.0.1", 60000,0,0);

	// Wait for the connection to complete
	for(;;)
	{
		packet = peer1->Receive();
		if (packet && packet->data[0]==ID_NEW_INCOMING_CONNECTION)
		{
			printf("Connected.\n");
			peer1->DeallocatePacket(packet);
			break;
		}
		peer1->DeallocatePacket(packet);
		RakSleep(0);
	}

	printf("Peer 2 sending ID_USER_PACKET_ENUM+1 and ID_USER_PACKET_ENUM\n");

	// Have peer 2 send a disallowed message, then the allowed message.
	message=static_cast<unsigned char>(ID_USER_PACKET_ENUM+1);
	peer2->Send(&message, 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);

	// Allowed message
	message=static_cast<unsigned char>(ID_USER_PACKET_ENUM);
	peer2->Send(&message, 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);

	RakSleep(1000);

	// Print out the messages that peer 1 got
	printf("We should get ID_USER_PACKET_ENUM and not ID_USER_PACKET_ENUM+1.\n");
	packet = peer1->Receive();
	while (packet)
	{
        switch (packet->data[0])
		{
		case ID_USER_PACKET_ENUM:
			printf("ID_USER_PACKET_ENUM\n");
			printf("User switched to group 1\n");
			// Switch the sender to group 1 now so that ID_USER_PACKET_ENUM+1 is allowed.
			messageFilter.SetSystemFilterSet(packet, 1);
			break;
		case ID_USER_PACKET_ENUM+1:
			printf("ID_USER_PACKET_ENUM+1\n");
			break;
		}
		peer1->DeallocatePacket(packet);
		RakSleep(0);
		packet = peer1->Receive();
	}

	// Have peer 2 send the messages again.
	message=static_cast<unsigned char>(ID_USER_PACKET_ENUM+1);
	peer2->Send(&message, 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	message=static_cast<unsigned char>(ID_USER_PACKET_ENUM);
	peer2->Send(&message, 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);

	RakSleep(1000);

	printf("We should now NOT get ID_USER_PACKET_ENUM and should get ID_USER_PACKET_ENUM+1\n");
	packet = peer1->Receive();
	while (packet)
	{
		switch (packet->data[0])
		{
		case ID_USER_PACKET_ENUM:
			printf("ID_USER_PACKET_ENUM\n");
			break;
		case ID_USER_PACKET_ENUM+1:
			printf("ID_USER_PACKET_ENUM+1\n");
			break;
		}
		peer1->DeallocatePacket(packet);
		packet = peer1->Receive();
		RakSleep(0);
	}

	printf("Done.\n");
	SLNet::RakPeerInterface::DestroyInstance(peer1);
	SLNet::RakPeerInterface::DestroyInstance(peer2);
	return 0;
}
