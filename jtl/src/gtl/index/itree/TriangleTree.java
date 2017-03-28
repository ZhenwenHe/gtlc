package gtl.index.itree;

import gtl.geom.Interval;
import gtl.geom.Vector;
import gtl.index.shape.IsoscelesRightTriangleShape;

import java.util.ArrayList;

/**
 * Created by ZhenwenHe on 2017/3/27.
 */
public class TriangleTree {
    /**
     * 树节点类，如果intervals==null，则为内部节点，
     * 否则为外部节点或叶子节点；
     * 当为内部节点的时候，left指向左子树节点，
     * right指向右子树节点；
     * parent指向父节点，如果父节点为空，则为根节点；
     * triangle是节点覆盖的三角形范围。
     */
    public class TreeNode {
        IsoscelesRightTriangleShape triangle;
        TreeNode parent;
        TreeNode left;
        TreeNode right;
        /**
         * if null, internal node
         * else external node , or leaf
         */
        ArrayList<Interval> intervals;

        public TreeNode() {
            this.triangle = null;
            this.parent = null;
            this.left = null;
            this.right = null;
            this.intervals = null;
        }

        boolean isLeafNode(){ return intervals==null;}
    }

    /**
     *树的根节点，其所包含的三角形范围为baseTriangle
     */
    TreeNode rootNode;
    /**
     * 基准三角形，记录根节点的范围，其始终是等腰直角三角形
     * V0为直角顶点，节点按照逆时针方向排列，分别V1，V2；
     * V0V1为X轴方向，V2V0为Y轴方向，作为基准坐标系的时候，V2为原点；
     * 基准三角形的范围可以左扩展（leftExtension），
     * 也可以右扩展（rightExtension），可以无限扩大；
     * 扩展后的基准三角形与原来的基准三角形为为相似三角形
     */
    IsoscelesRightTriangleShape baseTriangle;
    /**
     * 每个叶子节点中最多能存放leafNodeCapacity个间隔数据对象
     */
    int leafNodeCapacity;


    /**
     *
     * @param baseTriangle
     * @param leafNodeCapacity
     */
    public TriangleTree(IsoscelesRightTriangleShape baseTriangle,int leafNodeCapacity) {
        this.baseTriangle = (IsoscelesRightTriangleShape) baseTriangle.clone();
        this.leafNodeCapacity=leafNodeCapacity;
        this.rootNode=new TreeNode();
        this.rootNode.intervals=new ArrayList<>();
    }

    /**
     * 算法描述：
     * 1）调用findTreeNode查找i 要插入的节点tn（必定是叶子节点）
     * 2）如果tn的间隔数据对象个数小于leafNodeCapacity，则直接加入该节点
     * 3）如果tn中的间隔数据对象等于leafNodeCapacity，
     *      则执行节点分裂算法splitTreeNode，并将i插入
     *
     * @param i
     * @return
     */
    public boolean insert(Interval i){
        TreeNode tn = findTreeNode(i);
        if(tn.intervals.size()<leafNodeCapacity){
            tn.intervals.add(i);
        }
        else {
            return splitTreeNode(i,tn);
        }
        return true;
    }

    /**
     * 测试间隔数据对象i是否在基准三角形baseTriangle(直角等腰三角形)里面
     * 如果返回0，表示在三角形的外面；
     * 如果返回1，表示在基准三角形的左子三角形里面或边上
     * 如果返回2，则表示在基准三角形的右子三角形里面或边上；
     * @param  triangle
     * @param i
     * @return 0- out of triangle
     *          1-left sub triangle
     *          2- right sub triangle
     *          3- on one of the edges of this triangle
     */
    public int test(IsoscelesRightTriangleShape triangle, Interval i){
        return 0;
    }

    public boolean splitTreeNode(Interval i , TreeNode tn){
        return true;
    }

    /**
     * 查找待插入的节点，返回必定为叶子节点，
     * 如果为空则表示应该调用extend函数要进行基准三角形扩展
     * 算法描述：
     * 1）让p指向根节点
     * 2）测试p的三角形范围与间隔数据对象的位置关系
     * 3) 如果i在p的左三角形里面或边上，让p指向其左节点
     * 4）如果i在p的右三角形里面或边上，让p指向其右节点
     * 5）如果p是叶子节点，则返回p;否则跳转到2）
     * @param i
     * @return
     */
    TreeNode findTreeNode(Interval i){
        TreeNode p = rootNode;
        int testResult=0;
        while (p!=null){
            testResult=test(p.triangle,i);
            if(testResult==0)
                return null;
            else if(testResult==1){
                p = p.left;
            }
            else {//=2
                p=p.right;
            }
            if(p.isLeafNode())
                return p;
        }
        return null;
    }

    /**
     * 如果i不在baseTriangle里面或边上，则需要扩展baseTriangle
     *
     *
     * @param i
     * @return the new root node
     */
    TreeNode extend(Interval i){
        Vector V0 = rootNode.triangle.getVertex(0);
        if(i.getLowerBound()<=V0.getX())
            return leftExtension(rootNode);
        else if(i.getUpperBound()>=V0.getY())
            return rightExtension(rootNode);
        else
            return this.rootNode;
    }


    /**
     * 以传入的节点为基准三角形，进行范围扩展，
     * 并返回扩展后的父节点
     * @param tn
     * @return
     */
    TreeNode leftExtension(TreeNode tn){
        return null;
    }

    TreeNode rightExtension(TreeNode tn){
        return null;
    }
}
