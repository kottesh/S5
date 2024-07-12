var isEven = (num: number): boolean => {
  return num % 2 === 0;
};

let num: number = 32490324;
switch (isEven(num)) {
  case true:
    console.log(`${num} is Even`);
    break;
  default:
    console.log(`${num} is Odd`);
    break;
}
