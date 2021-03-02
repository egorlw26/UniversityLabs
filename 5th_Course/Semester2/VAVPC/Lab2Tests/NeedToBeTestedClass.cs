namespace Lab2Tests
{
    public class NeedToBeTestedClass : INeedToBeTestedClass
    {
        public double SomeDoubleOutput(double a, double b)
        {
            return (a + b) * (a + b);
        }

        public int SomeIntOutput(int input)
        {
            return input * input;
        }

        public string SomeOutputMethod()
        {
            return "I am original string";
        }
    }
}
