package cn.edu.cug.cs.study;

/**
 * Created by ZhenwenHe on 2016/12/1.
 */
public class BinaryTree {
    private class Node{
        private Node left;
        private Node right;
        private Comparable data;

        public Node(Comparable data) {
            this.data = data;
        }

        public Node getLeft() {
            return left;
        }

        public void setLeft(Node left) {
            this.left = left;
        }

        public Node getRight() {
            return right;
        }

        public void setRight(Node right) {
            this.right = right;
        }

        public Comparable getData() {
            return data;
        }

        public void setData(Comparable data) {
            this.data = data;
        }


        public void addNode(Node newNode){
            if(this.data.compareTo(newNode.getData())<0){
                if(this.left==null)
                    this.left=newNode;
                else{
                    this.left.addNode(newNode);
                }
            }
            else{
                if(this.right==null)
                    this.right=newNode;
                else
                    this.right.addNode(newNode);
            }
        }
    }
    private Node root;
    private int count;
    public void add(Object o){
        Comparable c = (Comparable)o;
        Node newNode = new Node(c);
        if(this.root==null)
            this.root=newNode;
        else
            this.root.addNode(newNode);
    }

}
