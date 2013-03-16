package com.example.ece381;

public class Packet {
	public static enum InfoType {
		STRING, CMD, TXT, AUDIO
	}
	public static final int HEADERSIZE = 2;
	private byte[] data;
	int data_size;
	Header header;
	/*
	 * Constructor of Packet from data achieved by Android
	 */
	public Packet(int data_size, byte[] data, int head, InfoType type, int num_packets) {
		init(data_size);
		this.header = new Header();
		setHeader(head, type, data_size+Packet.HEADERSIZE, num_packets);
		setData(data);
	}

	/*
	 * Constructor of Packet from data received through Middleman
	 */
	public Packet(byte[] data) {
		this.header = new Header();
		this.data = data;
		this.header.packet_length = (int)data[1];
		this.data_size = (int)data[1] - HEADERSIZE;
		if(this.getBit(data[0], 6) == 1) {
			this.header.isHead = true;
		} if(this.getBit(data[0], 5) == 1) {
			this.header.isEnd = true;
		} 
		switch((int)(data[0]&0x0F)) {
		case 0:
			this.header.packet_type = InfoType.STRING;
			break;
		case 1:
			this.header.packet_type = InfoType.CMD;
			break;
		case 2:
			this.header.packet_type = InfoType.TXT;
			break;
		case 3:
			this.header.packet_type = InfoType.AUDIO;
			break;
		default:
			break;
		}
		this.header.num_packets = 1;
	}
	/*
	 * initialize helper function
	 */
	public void init(int data_size) {
		this.data = new byte[data_size+Packet.HEADERSIZE];
		this.data_size = data_size;
	}
	/*
	 * set acknowledge
	 */
	public void setAck(int ack) {
		if(ack == 1)
			this.data[0] = insertBit(this.data[0], 7);
	}
	/*
	 * set up the header
	 */
	public void setHeader(int isHead, InfoType type, int size, int num_packets) {
		if(isHead == 0) {
			this.data[0] = insertBit(this.data[0], 6);
			this.header.isHead = true;
		}
		if(isHead == 2) {
			this.data[0] = insertBit(this.data[0], 5);
			this.header.isEnd = true;
		}

		this.header.packet_type = type;
		switch(type) {
		case STRING:
			this.data[0] = (byte) (this.data[0] & 0xF0);
			break;
		case CMD:
			this.data[0] = (byte) (this.data[0] | 0x01);
			break;
		case TXT:
			this.data[0] = (byte) (this.data[0] | 0x02);
			break;
		case AUDIO:
			this.data[0] = (byte) (this.data[0] | 0x03);
			break;
		default:
			break;
		}
		
		this.header.packet_length = size;
		this.data[1] = (byte)size;
		this.header.num_packets = num_packets;
	}
	
	public byte insertBit(byte b, int pos) {
		return (byte) (b|(1<<pos));
	}

	public int getBit(byte b, int pos) {
		return (b>>pos)&0x01;
	}
	/*
	 * return the array of bytes inside the packets
	 */
	public byte[] getPacket() {
		return this.data;
	}
	/*
	 * return the size of packets
	 */
	public int length() {
		return this.data_size;
	}
	/*
	 * return total packets number
	 */
	public int getPacketSize() {
		return this.header.num_packets;
	}
	public void setPacketSize(int s) {
		this.header.num_packets = s;
	}
	public boolean isEndPacket() {
		return this.header.isEnd;
	}
	/*
	 * return information type
	 */
	public InfoType getInfoType() {
		return this.header.packet_type;
	}
	public boolean setData(byte[] data) {
		if(data == null || data[0] == 0) return false;

		System.arraycopy(data, 0, this.data, Packet.HEADERSIZE, this.data_size);
		return true;
	}
	
	private class Header {
		private boolean isHead;
		private boolean isEnd;
		private int packet_length;
		private InfoType packet_type;
		private int num_packets;
		private Header() {
			this.isHead = this.isEnd = false;
		}
		
	}
}
