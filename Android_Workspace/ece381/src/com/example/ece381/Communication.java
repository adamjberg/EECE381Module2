package com.example.ece381;

import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

import android.util.Log;

public class Communication {
	public static enum Stats {
		START, checkServer, sendStates, waitServer, sendData0, waitAck0, sendData1,
				waitAck1, sendAck0, receiveData0, waitStart, startInit
	}
	
	private Stats statsMachine;
	private Queue<Packet> sendData;
	private Queue<Packet> receiveData;
	private Queue<Integer> pendingPacketSize;
	private static Communication RS232 = new Communication();
	private int client_ack, host_ack;
	private int currReceivePacketSize, index_packets;
	private int currSendPacketSize, index_send_packets;
	private boolean isReadySend;
	private int failReceveTime;
	private Packet PacketBuf;
	
	private Communication() {
		this.sendData = new ConcurrentLinkedQueue<Packet>();
		this.receiveData = new ConcurrentLinkedQueue<Packet>();
		this.pendingPacketSize = new ConcurrentLinkedQueue<Integer>();
		this.resetCom();
	}
	
	public void resetCom() {
		this.setState(Stats.START);
		this.client_ack = this.host_ack = 0;
		this.currReceivePacketSize = 1;
		this.currSendPacketSize = this.index_send_packets = 0;
		this.setIndex_packets(0);
		this.isReadySend = false;
		this.failReceveTime = 0;
		this.PacketBuf = null;
		this.sendData.clear();
		this.receiveData.clear();
		this.pendingPacketSize.clear();
	}
	static public Communication getInstance() {
		return RS232;
	}
	
	public boolean checkStartAck(byte startAck) {
		if((startAck & 0xF3) == 0xA3) {
			this.host_ack = getBit(startAck, 2);
			this.client_ack = getBit(startAck, 3);
			switchAck();
			return true;
		} return false;
	}
	
	public byte[] sendStartInit() {
		byte result[] = new byte[3];
		result[0] = (byte) 0xA3;
		result[1] = result[2] = (byte) 0xAA;
		
		updateAck(result);
		return result;
	}
	
	public void updateAck(byte[] bytes) {
		if(this.client_ack == 0) {
			bytes[0] &= 0xF7; //set client_ack bit to 0
		} else
			bytes[0] |= (1<<3); //set client_ack bit to 1
		
		if(this.host_ack == 0) {
			bytes[0] &= 0xFB; //set host_ack bit to 0
		} else
			bytes[0] |= (1<<2); //set host_ack bit to 1
	}
	/*
	 * Put the data on the queue to transmit
	 * return true if data is sending right away
	 * return false otherwise, data will be hold until it can be send 
	 */
	public boolean send(String data) {
		try {
			Queue<Packet> q = PacketConverter.convert(data);
			sendData.addAll(q);
			this.pendingPacketSize.add(Integer.valueOf(q.size()));
			if(this.isReadySend) {
				return false;
			} else {
				this.isReadySend = true; //notify that an information is ready to send
			}
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
		
		
		return true;
	}
	/*
	 * Pack received, put in queue if there are more packets to receive
	 */
	public void receive(Packet p) {
		try {
			p.setPacketSize(this.currReceivePacketSize);
			this.host_ack = p.getBit(p.getPacket()[0], 7);
			this.receiveData.add(p);
			this.setIndex_packets(this.getIndex_packets() + 1);
			this.setState(Stats.sendAck0);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	
	/*
	 * mutator; return sending queue
	 */
	public ConcurrentLinkedQueue<Packet> getSendQueue() {
		return (ConcurrentLinkedQueue<Packet>)this.sendData;
	}
	public Queue<Packet> getReceiveQueue() {
		return this.receiveData;
	}
	
	public void addPacketBuffer(Packet p) {
		this.PacketBuf = p;
	}
	/*
	 * get its current state
	 */
	public Stats getState() {
		return this.statsMachine;
	}
	
	public void setState(Stats s) {
		this.statsMachine = s;
		Log.i("Current State", s.toString());
	}

	/*
	 * This function send the state and acknowledge of current client
	 */
	public byte[] sendStats() {
		byte result[] = new byte[3];
		result[0] = (byte) 0xF0;
		result[1] = result[2] = 0;
		if(this.isReadySend || this.pendingPacketSize.size() > 0) {
			result[0] |= (1<<1);				
			this.index_send_packets = 0;
			this.isReadySend = true;
			this.currSendPacketSize = this.pendingPacketSize.poll().intValue();
			result[1] = (byte)(this.currSendPacketSize/256);
			result[2] = (byte)(this.currSendPacketSize%256);

			this.setState(Stats.waitServer);
		} else {
			result[0] |= (1<<0);
			this.setState(Stats.checkServer);
		}
		if(this.client_ack == 0) {
			result[0] &= 0xF7; //set client_ack bit to 0
		} else
			result[0] |= (1<<3); //set client_ack bit to 1
		
		if(this.host_ack == 0) {
			result[0] &= 0xFB; //set host_ack bit to 0
		} else
			result[0] |= (1<<2); //set host_ack bit to 1
		return result;
	}
	
	public byte[] sendAck() {
		byte result[] = new byte[3];
		result[0] = (byte) 0xF0;
		result[1] = result[2] = 0;
		
		if(this.client_ack == 0) {
			result[0] &= 0xF7; //set client_ack bit to 0
		} else
			result[0] |= (1<<3); //set client_ack bit to 1
		
		if(this.host_ack == 0) {
			result[0] &= 0xFB; //set host_ack bit to 0
		} else
			result[0] |= (1<<2); //set host_ack bit to 1
	
		result[0] |= (1 << 0);
		
		return result;
	}
	public void switchAck() {
		this.client_ack = (this.client_ack==0) ? 1 : 0;
	}
	
	public int getAck() {
		return this.client_ack;
	}
	/*
	 * Check to see if the acknowledge received from server has correct value
	 */
	public boolean checkAck(byte b) {
		if((b & 0xF0)== 0xF0 & (this.getBit(b, 3) == this.client_ack)) {
			this.host_ack = getBit(b, 2);
			this.switchAck();
			this.successfulReceived();
			return true;
		} this.failReceveTime++;
		return false;
	}
	/*
	 * Check to see if the acknowledge received from server has correct value
	 * Also check if more packets available to send
	 */
	public void checkAckAndQueue(byte b) {
		if(checkAck(b)) {
			this.PacketBuf = null; //free up the buffer
			if(this.index_send_packets < this.currSendPacketSize) {
				this.setState(Stats.sendData0);
			} else {
				this.index_send_packets = this.currSendPacketSize = 0;
				this.setState(Stats.sendStates);
				this.isReadySend = false;
			}
		} else {
			this.setState(Stats.sendData0);
		}
	}
	/*
	 * return times that has failed receiving since last sending
	 */
	public int getFailedTimes() {
		return this.failReceveTime;
	}
	/*
	 * increment the frequency of failing receive data; 
	 * go back to past state if it has fail 100 times
	 */
	public void failReceive(Stats pastState) {
		this.failReceveTime++;
		if(this.failReceveTime > 100) {
			this.setState(pastState);
			this.failReceveTime = 0;
		}
	}
	public void successfulReceived() {
		this.failReceveTime=0;
	}
	public int getBit(byte b, int pos) {
		return (b>>pos)&0x01;
	}
	public Packet getPacketBuf() {
		return this.PacketBuf;
	}
	public void setReceivePacketSize(int size) {
		this.currReceivePacketSize = size;
	}
	public int getReceivePacketSize() {
		return this.currReceivePacketSize;
	}

	public int getIndex_packets() {
		return index_packets;
	}

	public void setIndex_packets(int index_packets) {
		this.index_packets = index_packets;
	}
	public void setIndex_send_packets(int index_packets) {
		this.index_send_packets = index_packets;
	}
	public int getIndex_send_packets() {
		return this.index_send_packets;
	}
	public void setSendPacketSize(int size) {
		this.currSendPacketSize = size;
	}
	
}

