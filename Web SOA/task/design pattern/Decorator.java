abstract class Coffee {
    String desc;

    public String getDesc() {return desc;}
    public abstract double getCost();
}

abstract class CoffeeDecorator extends Coffee {
   public abstract String getDesc(); 
}

class WithMilk extends CoffeeDecorator  {
    Coffee coffee;

    public WithMilk(Coffee coffee) {
        this.coffee = coffee;
    }

    @Override
    public String getDesc() {
        return coffee.getDesc() + ", Milk";
    }

    @Override
    public double getCost() {
        return coffee.getCost() + .5;
    }
}

class WithSugar extends CoffeeDecorator {
    Coffee coffee;

    public WithSugar(Coffee coffee) {
        this.coffee = coffee;
    }
   
    @Override
    public String getDesc() {
        return coffee.getDesc() + ", Sugar";
    }

    @Override
    public double getCost() {
        return coffee.getCost() + .25;
    }
}

class Espresso extends Coffee {
    public Espresso() {
        desc = "Espresso";
    }

    @Override
    public double getCost() {
        return 2.99;
    }
}

public class Decorator {
    public static void main(String[] args) {
        Coffee espr = new Espresso();
        printCoffee(espr);

        espr = new WithMilk(espr);
        printCoffee(espr);

        espr = new WithSugar(espr);
        printCoffee(espr);
    }

    private static void printCoffee(Coffee coffee) {
        System.out.println("Coffee: " + coffee.getDesc() + "\nCost: " + coffee.getCost());
    }
}
