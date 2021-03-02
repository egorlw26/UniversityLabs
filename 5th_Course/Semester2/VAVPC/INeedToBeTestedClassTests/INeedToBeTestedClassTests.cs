using Lab2Tests;
using Moq;
using Xunit;

namespace INeedToBeTestedClassTests
{
    public class INeedToBeTestedClassTests
    {
        private NeedToBeTestedClass classInstance = new NeedToBeTestedClass();
        private Mock<INeedToBeTestedClass> classMock = new Mock<INeedToBeTestedClass>();
        private void SetupMock()
        {
            classMock.Setup(x => x.SomeOutputMethod()).Returns("I mocked output from tests");
            classMock.Setup(x => x.SomeIntOutput(It.IsAny<int>())).Returns((int a) => a + 1);
            classMock.Setup(x => x.SomeDoubleOutput(It.IsAny<double>(), It.IsAny<double>()))
                .Returns((double a, double b) => a + b);
        }


        [Fact]
        public void OriginalClassTestString()
        {
            var expected = "I am original string";
            Assert.Equal(expected, classInstance.SomeOutputMethod());
        }

        [Fact]
        public void OriginalClassTestInt()
        {
            var expected = 9;
            Assert.Equal(expected, classInstance.SomeIntOutput(3));
        }

        [Fact]
        public void OriginalClassTestDouble()
        {
            var expected = 25; //(2+3)^2
            Assert.Equal(expected, classInstance.SomeDoubleOutput(2, 3));
        }

        [Fact]
        public void MockClassTestString()
        {
            SetupMock();

            var expected = "I mocked output from tests";
            Assert.Equal(expected, classMock.Object.SomeOutputMethod());
        }

        [Fact]
        public void MockClassTestInt()
        {
            SetupMock();

            var expected = 2;
            Assert.Equal(expected, classMock.Object.SomeIntOutput(1));
        }

        [Fact]
        public void MockClassTestDouble()
        {
            SetupMock();

            var expected = 5;
            Assert.Equal(expected, classMock.Object.SomeDoubleOutput(2, 3));
        }
    }
}
