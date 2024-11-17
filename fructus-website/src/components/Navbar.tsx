"use client";

// Modules
import * as React from "react";
import Link from "next/link";
import Image from "next/image";

// Components
import { Button } from "@/components/ui/button";
import { Sheet, SheetContent, SheetTrigger } from "@/components/ui/sheet";

// Assets
import Logo from "@/assets/Logo.png";
import { DownloadCloud, Github, Menu } from "lucide-react";

const navItems = [
  { title: "Home", href: "/" },
  { title: "Sobre", href: "/sobre" },
  { title: "Download", href: "/download" },
];

export default function Navbar() {
  const [isOpen, setIsOpen] = React.useState(false);

  const handleDownload = () => {
    const filePath = "/fructus-software.zip";

    const link = document.createElement("a");
    link.href = filePath;
    link.download = "fructus-software.zip";
    link.click();
  };

  return (
    <nav className="py-4 w-full z-[10]">
      <div className="flex items-center justify-between px-4 mx-auto w-[90%] lg:w-[70%] tv:max-w-6xl sm:px-6 lg:px-8">
        <div className="flex items-center flex-shrink-0">
          <Link href="/" className="text-xl font-bold">
            <Image src={Logo} className="w-14" alt="Logo" />
          </Link>
        </div>

        <div className="hidden md:block bg-sub-dark rounded-2xl">
          <div className="px-6 py-2">
            {/* {navItems.map((item) => (
              <React.Fragment key={item.title}>
                <Link
                  href={item.href}
                  className="px-3 py-2 text-sm font-semibold rounded-xl text-[#575757]  hover:border-black/70 hover:text-black"
                >
                  {item.title}
                </Link>
              </React.Fragment>
            ))} */}
          </div>
        </div>

        <div className="flex items-center gap-2">
          <a
            href="https://github.com/Kc1t/Fructus/"
            target="_blank"
            rel="noopener noreferrer"
            className="z-20 items-center justify-center bg-[#FB9333] hover:bg-[#FB9333]/80 transition-all ease-in-out duration-300 h-11 w-11 md:h-10 md:w-10 cursor-pointer rounded-full hidden md:flex"
          >
            <Github className="text-white w-5 h-5" />
          </a>

          <Link href="/download">
            <Button
              onClick={handleDownload}
              className="hidden md:flex px-6 gap-2 rounded-full bg-[#FB9333] text-white hover:bg-[#FB9333]/80 font-medium"
            >
              <DownloadCloud className="w-5 text-white" />
              Baixar
            </Button>
          </Link>
          <Sheet open={isOpen} onOpenChange={setIsOpen}>
            <SheetTrigger asChild>
              <Button
                variant="ghost"
                size="icon"
                className="md:hidden text-black"
              >
                <Menu className="h-6 w-6 text-black" />
                <span className="sr-only">Fechar</span>
              </Button>
            </SheetTrigger>
            <SheetContent
              side="right"
              className="bg-white text-black border-black/20 border-l-2"
            >
              <div className="flex flex-col space-y-4 mt-4">
                {navItems.map((item) => (
                  <Link
                    key={item.title}
                    href={item.href}
                    className="px-3 py-2 text-sm font-semibold text-black"
                    onClick={() => setIsOpen(false)}
                  >
                    {item.title}
                  </Link>
                ))}
              </div>
            </SheetContent>
          </Sheet>
        </div>
      </div>
    </nav>
  );
}
