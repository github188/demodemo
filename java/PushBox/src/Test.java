public class Test {
    public static void main(String[] args) {
        byte[][] b1 = {{1,2},{3,4}};
        byte[][] b2 = b1.clone();
        for(int i = 0; i< b2.length; i++){
            b2[i] = b2[i].clone();
        }
        b1[0][0] = 100;
        System.out.println(b1[0][0] ==b2[0][0]);

    }
}
