import { Component } from '@angular/core';
import { RouterOutlet } from '@angular/router';
import { FormsModule } from '@angular/forms'; 

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [RouterOutlet, FormsModule],
  templateUrl: './app.component.html',
  styleUrl: './app.component.css'
})

export class AppComponent {
  title = 'SK';
  sname = 'SK';
  city = 'Coimbatore';

  display() {
    return "judice";
  }

  sum(a:number, b:number) {
    return a + b;
  }
}
