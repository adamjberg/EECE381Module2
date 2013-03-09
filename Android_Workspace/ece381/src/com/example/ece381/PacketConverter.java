package com.example.ece381;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

import com.example.ece381.Packet.InfoType;

public class PacketConverter {

	/*
	 * Decode a string into Packets without a header.  Return Queue of Packets if succeed
	 */
	public static Queue<Packet> convert(String msg) {
		// Create an array of bytes.  set data partition of Packets
		int length = msg.length();
		Queue<Packet> result = new ConcurrentLinkedQueue<Packet>();
		int i = 0;
		byte buf[] = new byte[100];
		int num_packets = length/100;
		if(length%100 > 0) {
			num_packets+=1;
		}
		int packet_size = 100;
		for(;i < num_packets; i++) {
			if(i == num_packets-1)
				if((packet_size = length%100) == 0)
					packet_size = 100;
			System.arraycopy(msg.getBytes(), i*100, buf, 0, packet_size);
			try {
				if(i == 0)
					result.add(new Packet(packet_size, buf, 0, InfoType.STRING, num_packets));
				else if(i == num_packets-1)
					result.add(new Packet(packet_size, buf, 2, InfoType.STRING, num_packets));
				else
					result.add(new Packet(packet_size, buf, 1, InfoType.STRING, num_packets));
			} catch (Exception e) {
				e.printStackTrace();
			}
			
		}
		return result;
	}
	
	public static Queue<Packet> convert(Command cmd) {
		
		return null;
	}
	public static Object decode(Queue<Packet> q) {
		switch(q.peek().getInfoType()) {
		case STRING:
			return decodeString(q);
		case CMD:
			return decodeCmd(q);
		case TXT:
			return null;
		case AUDIO:
			return null;
		default:
			return null;
		}
		
	}
	
	public static Object decodeString(Queue<Packet> q) {
		StringBuffer strBuf = new StringBuffer();
		Packet p;
		Iterator<Packet> iterator = q.iterator();
		while(iterator.hasNext()) {
			p = iterator.next();
			try {
				strBuf.append(new String(p.getPacket(), Packet.HEADERSIZE, p.length(), "US-ASCII"));
			} catch (UnsupportedEncodingException e) {
				e.printStackTrace();
			}
		}q.clear();
		return (Object)strBuf.toString();
	}
	
	public static Object decodeCmd(Queue<Packet> q) {
		ArrayList<Byte> dataBuf = new ArrayList<Byte>();

		Packet p;
		int i;
		int j = 0;
		int k;
		int len;
		Command cmd;
		Iterator<Packet> iterator = q.iterator();
		while(iterator.hasNext()) {
			p = iterator.next();
			for(i = 0; i < p.length(); i++) {
				dataBuf.add(new Byte(p.getPacket()[i+Packet.HEADERSIZE]));
			}
		}
		cmd = new Command(dataBuf.get(0).intValue());
		j = 2;
		byte tempBytes[];
		for(i = 0; i < dataBuf.get(1).intValue(); i++) {
			len = dataBuf.get(j++).intValue();
			tempBytes = new byte[len];
			for(k = 0; k < len; k++) {
				tempBytes[k] = dataBuf.get(j++).byteValue();
			}
			cmd.addParameter(new String(tempBytes));
		}
		q.clear();
		return (Object)cmd;
	}
}
