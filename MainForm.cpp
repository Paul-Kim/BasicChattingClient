
#include "TcpNetwork.h"
#include "ClientSceen.h"
#include "ClientSceenLogin.h"
#include "ClientSceenLobby.h"
#include "ClientScreenRoom.h"
#include "MainForm.h"


using PACKET_ID = NCommon::PACKET_ID;


MainForm::MainForm() {}

MainForm::~MainForm() 
{
	if (m_Network)
	{
		m_Network->DisConnect();
	}
}

void MainForm::Init()
{
	m_Network = std::make_unique<TcpNetwork>();

	m_pClientSceen = std::make_shared<ClientSceen>();
	m_pClientSceen->SetNetwork(m_Network.get());

	m_pClientSceenLogin = std::make_shared<ClientSceenLogin>();
	m_pClientSceenLogin->SetNetwork(m_Network.get());

	m_pClientSceenLobby = std::make_shared<ClientSceenLobby>();
	m_pClientSceenLobby->SetNetwork(m_Network.get());

	m_pClientSceenRoom = std::make_shared<ClientSceenRoom>();
	m_pClientSceenRoom->SetNetwork(m_Network.get());
}

void MainForm::CreateGUI()
{
	// https://moqups.com/   여기에서 디자인 하자

	m_fm = std::make_unique<form>(API::make_center(900, 700));
	m_fm->caption("Chat Client");

	m_pClientSceen->CreateUI(m_fm.get());
	
	m_pClientSceenLogin->CreateUI(m_fm.get());

	m_pClientSceenLobby->CreateUI(m_fm.get());

	m_pClientSceenRoom->CreateUI(m_fm.get());
	
	m_timer.elapse([&]() { PacketProcess();});
	m_timer.interval(32);
	m_timer.start();
}

void MainForm::ShowModal()
{
	m_fm->show();

	exec();
}

void MainForm::PacketProcess()
{
	if (!m_Network) {
		return;
	}

	
	auto packet = m_Network->GetPacket();

	if (packet.PacketId != 0)
	{
		m_pClientSceen->ProcessPacket(packet.PacketId, packet.pData);
		m_pClientSceenLogin->ProcessPacket(packet.PacketId, packet.pData);
		m_pClientSceenLobby->ProcessPacket(packet.PacketId, packet.pData);
		m_pClientSceenRoom->ProcessPacket(packet.PacketId, packet.pData);

		if (packet.pData != nullptr) {
			delete[] packet.pData;
		}
	}
	

	m_pClientSceen->Update();
	m_pClientSceenLogin->Update();
	m_pClientSceenLobby->Update();
	m_pClientSceenRoom->Update();
}

