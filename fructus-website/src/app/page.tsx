"use client"

// Components
import Navbar from "@/components/Navbar";
import { Button } from "@/components/ui/button";

// Assets
import { DownloadCloud } from "lucide-react";
import Link from "next/link";

export default function Page() {
  const handleDownload = () => {
    const filePath = "/fructus-software.zip";

    const link = document.createElement("a");
    link.href = filePath;
    link.download = "fructus-software.zip";
    link.click();
  };
  return (
    <div className="bg-[#FDF1F5] w-full min-h-screen tv:h-auto overflow-x-hidden flex flex-col justify-between tv:justify-center items-center">
      <div className="h-[70vh] tv:h-[60vh] w-full overflow-hidden flex flex-col justify-between items-center">
        <Navbar />
        <div className="text-center flex flex-col lg:gap-2 z-[10] w-full">
          <div className="my-3">
            <span className="bg-[#FFF0DC] text-[#FB9333] p-2 px-4 rounded-full font-semibold">
              Software feito em C
            </span>
          </div>

          <h1 className="text-2xl font-bold tracking-wide sm:text-5xl text-[#191919] md:text-5xl">
            <span className="block xl:inline">
              <span className="mx-1 text-[rgb(251,147,51)]">Administre</span>
              suas <br /> operações com Fructus
            </span>
          </h1>
          <p className="mx-auto mt-3 max-w-4xl text-sm w-5/6 md:w-full lg:text-xl text-[#252525] sm:mt-5 md:mt-2">
            Fructus é um software em C para um hortifruti fictício, desenvolvido
            no PIM II. Este site foi criado para baixar os arquivos e oferecer
            uma visão mais detalhada do projeto.
          </p>
          <Link href="/download" className="my-2">
            <Button onClick={handleDownload} className="bg-[#252525] text-sm md:text-base text-white hover:bg-[#252525]/80 rounded-full px-6 py-6 md:py-7">
              Baixar Repositório{" "}
              <div className="bg-white mx-2 rounded-full h-8 w-8 flex items-center justify-center">
                <DownloadCloud className="text-black w-4" />
              </div>{" "}
            </Button>
          </Link>
        </div>
        <div></div>
      </div>
      <div className="my-4 px-2 w-full max-w-6xl flex items-center justify-between flex-col">
        <div className="bg-[#F5F5F5] w-full h-8 md:h-12 flex justify-center md:justify-between items-center px-4">
          <div className="hidden md:flex gap-1 items-center justify-center">
            <div className="h-4 w-4 bg-[#D83B3B] rounded-full"></div>
            <div className="h-4 w-4 bg-[#E2C423] rounded-full"></div>
            <div className="h-4 w-4 bg-[#03CA0B] rounded-full"></div>
          </div>
          <div className="font-semibold text-xs">
            hortifruti@fructus — 80 x 24
          </div>
          <div></div>
        </div>
        <video
          className=""
          controls
          src="https://cdn-cf-east.streamable.com/video/mp4/fgi0fq.mp4?Expires=1726000988287&amp;Key-Pair-Id=APKAIEYUVEN4EVB2OKEQ&amp;Signature=eYnlhXp-OXDsFA9N6fMSn4fqHydG-pgsrhVw3XW0wrv6YrM35kudgSbuPNyrUDu5jNc-6FmckBx~6j4~JEl6QLm0p9Ea45fETY5UzOHpmQRycZxaRZRaqhbIPgwf7EdJ-P8O3bgcttQYH6nnuVTmwbpicdhV00q9LfRYPn7ZwI7S0cNFxHXfg4MCvbAu0lQ86UP7HWqZ4Zmz6gq8ZprDKTZ~jpKAavZhoesKFUaVR5-Z3IyrYjhw2MfwQ~mnHa7z~gCEFW-OSIkGo3VieJpND5ewHdoOQtF2H0XnVzgGVWZZR1N14lhniQ479~cSuFBvUiECbqp5wlQibNkZ8hu~Dg__"
        ></video>
      </div>
    </div>
  );
}
