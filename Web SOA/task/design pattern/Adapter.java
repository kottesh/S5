// This adapter DP demonstrates how the old student grade gets
// adapted to the new way of display the grade.

interface Student {
    String getName();
    char getGrade(); 
}

// Old method of storing student grade(i.e., Percentage). 
class LegacyStudent {
    private String fullname;
    private double percentage;

    public LegacyStudent(String fullname, double percentage) {
        this.fullname = fullname;
        this.percentage = percentage;
    }

    public String getName() {return fullname;}
    public double getPercentage() {return percentage;}
}

// student adapter class for the legacy student details.
class StudentAdapter implements Student {
    private LegacyStudent student;

    public StudentAdapter(LegacyStudent student) { this.student = student; }

    @Override
    public String getName() { return student.getName(); }

    @Override
    public char getGrade() {
        double percentage = student.getPercentage();
        if (percentage >= 90) return 'A';
        if (percentage >= 80) return 'B';
        if (percentage >= 70) return 'C';
        if (percentage >= 60) return 'D';
        return 'F';
    }
} 

class Student_ implements Student {
    private String name;
    private char grade;

    Student_(String name, char grade) {
        this.name = name;
        this.grade = grade;
    }

    @Override
    public String getName() { return name; }

    @Override
    public char getGrade() { return grade; }
}

interface StudentFactory {
    Student createStudent(String name, double gradeOrPercentage);
}

class LegacyStudentFactory implements StudentFactory {
    @Override
    public Student createStudent(String name, double gradeOrPercentage) {
        return new StudentAdapter(new LegacyStudent(name, gradeOrPercentage));
    }
}

class StudentFactory_ implements StudentFactory {
    @Override
    public Student createStudent(String name, double gradeOrPercentage) {
        return new Student_(name, (char)gradeOrPercentage);
    }
}

public class Adapter {
    public static void main(String[] args) {
        LegacyStudentFactory lsf = new LegacyStudentFactory(); 
        StudentFactory_ sf = new StudentFactory_();

        Student legacyStudent = lsf.createStudent("Louise", 20);
        System.out.println("Legacy\nName: " + legacyStudent.getName());
        System.out.println("Grade: " + legacyStudent.getGrade());

        Student newStudent = sf.createStudent("Ken", 'A');
        System.out.println("New\nName: " + newStudent.getName());
        System.out.println("Grade: " + newStudent.getGrade());
    }
}

