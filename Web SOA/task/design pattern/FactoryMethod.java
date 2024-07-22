interface Product {
    void operation();
}

class Product1 implements Product {
    @Override
    public void operation() {
        System.out.println("P1 Operation");
    }
}

class Product2 implements Product {
    @Override
    public void operation() {
        System.out.println("P2 Operation");
    }
}

abstract class Creator {
    public abstract Product createProduct();

    public Product orderProduct() {
        Product product = createProduct();
        return product;
    }
}

class Creator1 extends Creator {
    public Product createProduct() {
        return new Product1();
    }
} 

class Creator2 extends Creator {
    public Product createProduct() {
        return new Product2();
    }
}

public class FactoryMethod {
    public static void main(String[] args) {
        Creator cr1 = new Creator1();
        Product p1 = cr1.orderProduct();
        p1.operation();

        Creator cr2 = new Creator2();
        Product p2 = cr2.orderProduct();
        p2.operation();
    }
}
