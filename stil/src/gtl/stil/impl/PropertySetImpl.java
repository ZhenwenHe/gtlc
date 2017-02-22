package gtl.stil.impl;

import gtl.stil.*;

import java.io.*;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

/**
 * Created by ZhenwenHe on 2016/12/27.
 */
public class PropertySetImpl extends HashMap<String,Variant> implements PropertySet{

    public PropertySetImpl() {
    }

    public PropertySetImpl(byte [] data){
        try {
            loadFromByteArray(data);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void put(Property p) {
        super.put(p.getName(),(Variant) p);
    }

    @Override
    public void copyFrom(Object i) {
        if(i instanceof PropertySet) {
            super.clear();
            for (Map.Entry<String, Variant> es : ((PropertySet)i).entrySet()) {
                super.put(es.getKey(), es.getValue());
            }
        }
    }

    @Override
    public boolean read(InputStream in) throws IOException {
        super.clear();
        DataInputStream dis =new DataInputStream(in);
        int count = dis.readInt();
        String key=null;
        Variant value=null;
        for(int i=0;i<count;++i){
            //read key string
            key=Variant.readString(dis);
            value =new Variant();
            value.read(in);
            super.put(key,value);
        }
        return true;
    }

    @Override
    public boolean write(OutputStream out) throws IOException {
        DataOutputStream dos =new DataOutputStream(out);
        int c = this.size();
        dos.writeInt(c);
        for(Map.Entry<String,Variant> es:entrySet()){
            Variant.writeString(dos,(String)(es.getKey()));
            ((Variant)es.getValue()).write(out);
        }

        dos.close();
        return true;
    }

    @Override
    public long getByteArraySize() {
        int c =  4 ;// entry count;
        String s =null;
        for(Map.Entry<String,Variant> es:entrySet()){
            s=(String)(es.getKey());
            c+=4;//key string length
            c+=s.length()*2;
            c+=((Variant)es.getValue()).getByteArraySize();
        }


        return c;
    }
    @Override
    public Variant getProperty(String key){
        return super.get((Object)key);
    }
}
