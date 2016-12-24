package gtl.stil.rtree.impl;

import java.io.*;
import java.util.ArrayList;

/**
 * Created by ZhenwenHe on 2016/12/16.
 */
public class StatisticsImpl implements gtl.stil.Statistics{
    public StatisticsImpl() {
        reset();
    }

    public void setReadTimes(long readTimes) {
        this.readTimes = readTimes;
    }

    public void setWriteTimes(long writeTimes) {
        this.writeTimes = writeTimes;
    }

    public long getSplitTimes() {
        return splitTimes;
    }

    public void setSplitTimes(long splitTimes) {
        this.splitTimes = splitTimes;
    }

    public long getHits() {
        return hits;
    }

    public void setHits(long hits) {
        this.hits = hits;
    }

    public long getMisses() {
        return misses;
    }

    public void setMisses(long misses) {
        this.misses = misses;
    }

    public void setNodeNumber(long nodeNumber) {
        this.nodeNumber = nodeNumber;
    }

    public long getAdjustments() {
        return adjustments;
    }

    public void setAdjustments(long adjustments) {
        this.adjustments = adjustments;
    }

    public long getQueryResults() {
        return queryResults;
    }

    public void setQueryResults(long queryResults) {
        this.queryResults = queryResults;
    }

    public void setDataNumber(long dataNumber) {
        this.dataNumber = dataNumber;
    }

    public long getTreeHeight() {
        return treeHeight;
    }

    public void setTreeHeight(long treeHeight) {
        this.treeHeight = treeHeight;
    }

    public ArrayList<Long> getNodeNumberInLevelArray() {
        return nodeNumberInLevelArray;
    }

    public void setNodeNumberInLevelArray(ArrayList<Long> nodeNumberInLevelArray) {
        this.nodeNumberInLevelArray = nodeNumberInLevelArray;
    }

    public long getNodeNumberInLevel(int level) {
        return nodeNumberInLevelArray.get(level);
    }
    public void setNodeNumberInLevel(int level,long numb) {
        nodeNumberInLevelArray.set(level,numb);
    }
    @Override
    public long getReadTimes() {
        return this.readTimes;
    }

    @Override
    public long getWriteTimes() {
        return this.writeTimes;
    }

    @Override
    public Object clone() {
        StatisticsImpl s = new StatisticsImpl();
        copyTo(s);
        return s;
    }

    @Override
    public long getNodeNumber() {
        return this.nodeNumber;
    }

    @Override
    public void copyFrom(Object i) {
        if(i instanceof StatisticsImpl) {
            StatisticsImpl s = (StatisticsImpl) (i);
            this.readTimes = s.readTimes;
            this.writeTimes = s.writeTimes;
            this.splitTimes = s.splitTimes;
            this.hits = s.hits;
            this.misses = s.misses;
            this.nodeNumber = s.nodeNumber;
            this.adjustments = s.adjustments;
            this.queryResults = s.queryResults;
            this.dataNumber = s.dataNumber;
            this.treeHeight=s.treeHeight;
            this.nodeNumberInLevelArray = new ArrayList<Long>(s.nodeNumberInLevelArray);
        }
    }

    @Override
    public long getDataNumber() {
        return this.dataNumber;
    }

    @Override
    public void copyTo(Object i) {
        if(i instanceof StatisticsImpl) {
            StatisticsImpl s = (StatisticsImpl) (i);
            s.readTimes = this.readTimes;
            s.writeTimes = this.writeTimes;
            s.splitTimes = this.splitTimes;
            s.hits = this.hits;
            s.misses = this.misses;
            s.nodeNumber = this.nodeNumber;
            s.adjustments = this.adjustments;
            s.queryResults = this.queryResults;
            s.dataNumber = this.dataNumber;
            s.treeHeight=this.treeHeight;
            s.nodeNumberInLevelArray = new ArrayList<Long>(this.nodeNumberInLevelArray);
        }
    }

    @Override
    public boolean read(InputStream in) throws IOException {
        DataInputStream dis =new DataInputStream(in);
        this.readTimes=dis.readLong();
        this.writeTimes=dis.readLong();
        this.splitTimes=dis.readLong();
        this.hits=dis.readLong();
        this.misses=dis.readLong();
        this.nodeNumber=dis.readLong();
        this.adjustments=dis.readLong();
        this.queryResults=dis.readLong();
        this.dataNumber=dis.readLong();
        this.treeHeight=dis.readLong();
        int s = dis.readInt();
        for(int i=0;i<s;i++){
            this.nodeNumberInLevelArray.add(Long.valueOf(dis.readLong()));
        }
        dis.close();
        return true;
    }

    @Override
    public boolean write(OutputStream out) throws IOException {
        DataOutputStream dos =new DataOutputStream(out);
        dos.writeLong(this.readTimes);
        dos.writeLong(this.writeTimes);
        dos.writeLong(this.splitTimes);
        dos.writeLong(this.hits);
        dos.writeLong(this.misses);
        dos.writeLong(this.nodeNumber);
        dos.writeLong(this.adjustments);
        dos.writeLong(this.queryResults);
        dos.writeLong(this.dataNumber);
        dos.writeLong(this.treeHeight);
        int s = this.nodeNumberInLevelArray.size();
        dos.writeInt(s);
        for(Long i: this.nodeNumberInLevelArray){
            dos.writeLong(i);
        }
        dos.close();
        return true;
    }


    @Override
    public long getByteArraySize() {
        return (10+this.nodeNumberInLevelArray.size())*8+4;
    }

    @Override
    public void reset() {
        readTimes=0;
        writeTimes=0;
        splitTimes=0;
        hits=0;
        misses=0;
        nodeNumber=0;
        adjustments=0;
        queryResults=0;
        dataNumber=0;
        treeHeight=0;
        nodeNumberInLevelArray =new ArrayList<Long> ();
    }


    long readTimes;

    long writeTimes;

    long splitTimes;

    long hits;

    long misses;

    long nodeNumber;

    long adjustments;

    long queryResults;

    long dataNumber;

    long treeHeight;

    ArrayList<Long> nodeNumberInLevelArray;
}
