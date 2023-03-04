<!-- Improved compatibility of back to top link: See: https://github.com/itsmanjeet/bolt/pull/73 -->
<a name="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]


<!-- PROJECT LOGO -->
<br />
<div align="center">
<h3 align="center">Bolt</h3>

  <p align="center">
    A flexible personal virtual assistant
    <br />
    <a href="https://github.com/itsmanjeet/bolt"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/itsmanjeet/bolt/issues">Report Bug</a>
    ·
    <a href="https://github.com/itsmanjeet/bolt/issues">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->

## About The Project

Bolt is a simple, easy-to-use, lightweight, flexible virtual assistant primarily developed
for [rlxos GNU/Linux](https://rlxos.dev) but can work on any Unix based operating system, Primary target behind
developing a lightweight and easy-to-use virtual assistant that any user can train and work with. Bolt uses the ML
classification algorithm to detect the user intention and perform task accordingly. Bolt functionality can be extended
using c++ dynamic modules, shell scripts or python scripts.

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->

## Getting Started

### Prerequisites

* C++17 compiler (g++10/clang++12)
* libdl (glibc or any other unix libc)
* CMake & ninja

### Build

1. Clone the repo
   ```sh
   git clone https://github.com/itsmanjeet/bolt.git
   ```
2. Compile the project using cmake
   ```sh
   cmake -B build -DCMAKE_INSTALL_PREFIX=/usr
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->

## Usage

**Test Run**
```shell
./build/bolt                            \
      -model ./data/model.json          \
      -responses ./data/responses.txt   \
      -plugin-path ./build/plugins      \
      hey their

```

**Training model**
```shell
./build/bolt                            \
      -model ./data/model.json          \
      -responses ./data/responses.txt   \
      -plugin-path ./build/plugins      \
      -train ./training.txt

```


_For more examples, please refer to the [Documentation](https://bolt.rlxos.dev)_

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- ROADMAP -->

## Roadmap

- [x] Implementation of NaiveBayes classification algorithm
- [x] Use FuzzySearch for unknown queries
- [x] Handle builtins using '@' prefix
- [ ] Add TTS support (using espeak, festival)
- [ ] Add STT support (...)
- [ ] Multi-language Support
    - [ ] ...

See the [open issues](https://github.com/itsmanjeet/bolt/issues) for a full list of proposed features (
and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTRIBUTING -->

## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any
contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also
simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- LICENSE -->

## License

Distributed under the MIT License. See `LICENSE` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->

## Contact

Manjeet Singh - [@rlxos_dev](https://twitter.com/rlxos_dev) - itsmanjeet1998@gmail.com

Project Link: [https://github.com/itsmanjeet/bolt](https://github.com/itsmanjeet/bolt)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- ACKNOWLEDGMENTS -->

## Special Thanks

![JetBrains Logo (Main) logo](https://resources.jetbrains.com/storage/products/company/brand/logos/jb_beam.png)
[JetBrain OpenSource Support](https://jb.gg/OpenSourceSupport)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[contributors-shield]: https://img.shields.io/github/contributors/itsmanjeet/bolt.svg?style=for-the-badge

[contributors-url]: https://github.com/itsmanjeet/bolt/graphs/contributors

[forks-shield]: https://img.shields.io/github/forks/itsmanjeet/bolt.svg?style=for-the-badge

[forks-url]: https://github.com/itsmanjeet/bolt/network/members

[stars-shield]: https://img.shields.io/github/stars/itsmanjeet/bolt.svg?style=for-the-badge

[stars-url]: https://github.com/itsmanjeet/bolt/stargazers

[issues-shield]: https://img.shields.io/github/issues/itsmanjeet/bolt.svg?style=for-the-badge

[issues-url]: https://github.com/itsmanjeet/bolt/issues

[license-shield]: https://img.shields.io/github/license/itsmanjeet/bolt.svg?style=for-the-badge

[license-url]: https://github.com/itsmanjeet/bolt/blob/master/LICENSE.txt
